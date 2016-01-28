/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "ship.h"
#include "game_data.h"
#include "game.h"
#include "game_constants.h"

#include <engine.h>
#include <game_manager.h>
#include <sound_manager.h>
#include <render.h>

#include <unordered_set>

using namespace std;

Ship::Ship(string new_type,const Coords<double>& position,double new_angle){
    type=new_type;

    box.x=position.x;
    box.y=position.y;

    angle=new_angle;

    hull=get_hull_max();
    shields=get_shields_max();

    thrusting=false;
    braking=false;

    landing=false;
    landing_scale=1.0;
    landing_planet_index=0;

    shield_recharge=0;

    weapons_enabled=true;
    weapon_cooldown=0;

    laser_has_target=false;

    disabled_cooldown=0;

    sprite.set_name(get_ship_type()->sprite);

    box.w=sprite.get_width();
    box.h=sprite.get_height();

    if(get_ship_type()->weapon.length()>0){
        add_upgrade(get_ship_type()->weapon);
    }

    if(get_ship_type()->active.length()>0){
        add_upgrade(get_ship_type()->active);
    }

    ai_target_next=position;
    clear_proximity_target();
}

Ship_Type* Ship::get_ship_type() const{
    return Game_Data::get_ship_type(type);
}

Collision_Rect<double> Ship::get_box() const{
    return box;
}

Vector Ship::get_velocity() const{
    return velocity;
}

double Ship::get_angle() const{
    return angle;
}

int32_t Ship::get_hull() const{
    return hull;
}

int32_t Ship::get_shields() const{
    return shields;
}

uint32_t Ship::get_shield_recharge_rate() const{
    return Game_Constants::SHIELD_RECHARGE_RATE;
}

bool Ship::has_upgrade(string name) const{
    for(size_t i=0;i<upgrades.size();i++){
        if(upgrades[i]==name){
            return true;
        }
    }

    return false;
}

bool Ship::has_weapon() const{
    return get_weapon_name().length()>0;
}

string Ship::get_weapon_name() const{
    for(size_t i=0;i<upgrades.size();i++){
        if(Game_Data::get_upgrade_type(upgrades[i])->is_weapon()){
            return upgrades[i];
        }
    }

    return "";
}

void Ship::remove_weapon(){
    if(has_weapon()){
        remove_upgrade(get_weapon_name());
    }
}

bool Ship::has_active() const{
    return get_active_name().length()>0;
}

string Ship::get_active_name() const{
    for(size_t i=0;i<upgrades.size();i++){
        if(Game_Data::get_upgrade_type(upgrades[i])->is_active()){
            return upgrades[i];
        }
    }

    return "";
}

void Ship::remove_active(){
    if(has_active()){
        remove_upgrade(get_active_name());
    }
}

bool Ship::are_weapons_enabled() const{
    return weapons_enabled;
}

void Ship::toggle_weapons(){
    weapons_enabled=!weapons_enabled;
}

void Ship::add_upgrade(string name){
    if(!has_upgrade(name)){
        Upgrade* upgrade=Game_Data::get_upgrade_type(name);

        if(upgrade->is_weapon()){
            remove_weapon();
        }
        else if(upgrade->is_active()){
            remove_active();
        }

        upgrades.push_back(name);
    }
}

void Ship::remove_upgrade(string name){
    for(size_t i=0;i<upgrades.size();){
        if(upgrades[i]==name){
            upgrades.erase(upgrades.begin()+i);
        }
        else{
            i++;
        }
    }
}

bool Ship::has_laser() const{
    return has_upgrade("plasma_gun");
}

void Ship::calculate_laser_target(const Quadtree<double,uint32_t>& quadtree_ships,uint32_t own_index){
    bool is_player=own_index==0;

    if(is_alive() && !is_landing() && (!is_player || !Game::player_is_landed()) && !is_disabled(is_player)){
        if(weapons_enabled && has_laser()){
            Collision_Rect<double> box_targeting=box;

            Upgrade* upgrade=Game_Data::get_upgrade_type(get_weapon_name());

            box_targeting.x-=upgrade->range;
            box_targeting.y-=upgrade->range;
            box_targeting.w+=upgrade->range*2.0;
            box_targeting.h+=upgrade->range*2.0;

            int32_t nearest_index=get_nearest_valid_target_ship(quadtree_ships,own_index,box_targeting,true);

            //If a nearest valid target was found
            if(nearest_index>=0){
                const Ship& ship=Game::get_ship((uint32_t)nearest_index);

                laser_has_target=true;

                laser_endpoint.x=ship.get_box().center_x();
                laser_endpoint.y=ship.get_box().center_y();

                return;
            }
        }
    }

    laser_has_target=false;
}

void Ship::render_laser(bool is_player){
    if(is_alive() && !is_landing() && (!is_player || !Game::player_is_landed()) && !is_disabled(is_player)){
        if(laser_has_target && (Collision::check_rect(box*Game_Manager::camera_zoom,Game_Manager::camera) ||
                                Collision::check_rect(Collision_Rect<double>(laser_endpoint.x,laser_endpoint.y,1.0,1.0)*Game_Manager::camera_zoom,Game_Manager::camera))){
            Render::render_line(box.center_x()*Game_Manager::camera_zoom-Game_Manager::camera.x,box.center_y()*Game_Manager::camera_zoom-Game_Manager::camera.y,
                                laser_endpoint.x*Game_Manager::camera_zoom-Game_Manager::camera.x,laser_endpoint.y*Game_Manager::camera_zoom-Game_Manager::camera.y,1.0,"laser");
        }
    }
}

bool Ship::is_disabled(bool is_player) const{
    return disabled_cooldown>0 || (is_player && Game::player_is_out_of_power());
}

void Ship::disable(){
    disabled_cooldown=Game_Constants::DISABLED_LENGTH*Engine::UPDATE_RATE;

    thrusting=false;
}

bool Ship::can_use_item(Item_Type* item_type) const{
    if(item_type->restores_hull()){
        return get_hull()<get_hull_max();
    }
    else if(item_type->restores_power()){
        return Game::get_power()<Game_Constants::MAX_POWER*Engine::UPDATE_RATE;
    }
    else{
        return true;
    }
}

void Ship::use_item(Item_Type* item_type){
    if(item_type->restores_hull()){
        hull+=Game_Constants::ITEM_RESTORE_HULL;

        if(hull>get_hull_max()){
            hull=get_hull_max();
        }
    }
    else if(item_type->restores_power()){
        Game::increase_power();
    }

    Game::increase_score(item_type->point_value);
}

void Ship::apply_tractor(double force_angle){
    Vector tractor_force(Game_Constants::TRACTOR_FORCE,force_angle);

    force+=tractor_force;
}

double Ship::get_thrust_accel() const{
    return get_ship_type()->thrust_accel;
}

double Ship::get_thrust_decel() const{
    return get_ship_type()->thrust_decel;
}

double Ship::get_max_speed() const{
    return get_ship_type()->max_speed;
}

int32_t Ship::get_hull_max() const{
    return get_ship_type()->hull_max;
}

int32_t Ship::get_shields_max() const{
    return get_ship_type()->shields_max;
}

string Ship::get_faction() const{
    return get_ship_type()->faction;
}

Collision_Rect<double> Ship::get_collision_box() const{
    return Collision_Rect<double>(box.x+box.w/2.0-box.w*get_ship_type()->collision_percentage/2.0,box.y+box.h/2.0-box.h*get_ship_type()->collision_percentage/2.0,
                                  box.w*get_ship_type()->collision_percentage,box.h*get_ship_type()->collision_percentage);
}

bool Ship::is_alive() const{
    return hull>0;
}

double Ship::get_distance_to_player() const{
    const Ship& player=Game::get_player_const();

    return Math::distance_between_points(box.center_x(),box.center_y(),player.get_box().center_x(),player.get_box().center_y());
}

double Ship::get_distance_to_proximity_target() const{
    if(ai_has_proximity_target){
        const Ship& ship=Game::get_ship(ai_proximity_target);

        return Math::distance_between_points(box.center_x(),box.center_y(),ship.get_box().center_x(),ship.get_box().center_y());
    }
    else{
        return 0.0;
    }
}

bool Ship::was_damaged_by_explosion(uint32_t index) const{
    for(size_t i=0;i<explosions.size();i++){
        if(index==explosions[i]){
            return true;
        }
    }

    return false;
}

void Ship::damaged_by_explosion(uint32_t index){
    explosions.push_back(index);
}

void Ship::notify_of_explosion_death(uint32_t index){
    for(size_t i=0;i<explosions.size();){
        if(index==explosions[i]){
            explosions.erase(explosions.begin()+i);
        }
        else{
            if(index<explosions[i]){
                explosions[i]--;
            }

            i++;
        }
    }
}

void Ship::clear_proximity_target(){
    ai_proximity_target=0;
    ai_has_proximity_target=false;
    ai_proximity_target_flee=false;
}

void Ship::notify_of_ship_death(uint32_t index){
    if(ai_has_proximity_target){
        if(index==ai_proximity_target){
            clear_proximity_target();
        }
        else if(index<ai_proximity_target){
            ai_proximity_target--;
        }
    }
}

void Ship::take_damage(bool is_player,int32_t damage,string damage_type,const Coords<double>& location,string damage_faction,RNG& rng){
    if(is_alive()){
        if(damage_faction=="player" && !is_player && (get_faction()=="civilian" || get_faction()=="police")){
            Game::increase_notoriety();
        }

        int32_t effective_damage=damage;

        if(shields>0){
            if(damage_type=="solid"){
                effective_damage*=2;
            }
            else if(damage_type=="explosive"){
                effective_damage=int32_t((double)effective_damage*1.5);
            }
            else if(damage_type=="energy"){
                effective_damage/=2;
            }

            if(effective_damage>0){
                double scale=((double)effective_damage/(double)get_shields_max())/2.0;
                if(scale>1.0){
                    scale=1.0;
                }
                else if(scale<0.5){
                    scale=0.5;
                }

                Game::create_effect("effect_explosion_shields",scale,location,"effect_explosion_shields",
                                    Vector(0.0,0.0),0.0,Vector(0.0,0.0),0);

                if(effective_damage<=shields){
                    shields-=effective_damage;

                    return;
                }
                else{
                    effective_damage-=shields;

                    shields=0;

                    if(damage_type=="solid"){
                        effective_damage/=2;
                    }
                    else if(damage_type=="explosive"){
                        effective_damage=int32_t((double)effective_damage/1.5);
                    }
                    else if(damage_type=="energy"){
                        effective_damage*=2;
                    }
                }
            }
            else{
                return;
            }
        }

        if(damage_type=="solid"){
            effective_damage/=2;
        }
        else if(damage_type=="explosive"){
            effective_damage=int32_t((double)effective_damage*1.5);
        }
        else if(damage_type=="energy"){
            effective_damage*=2;
        }

        if(effective_damage>0){
            double scale=((double)effective_damage/(double)get_hull_max())/2.0;
            if(scale>1.0){
                scale=1.0;
            }
            else if(scale<0.5){
                scale=0.5;
            }

            Game::create_effect("effect_explosion_hull",scale,location,"effect_explosion_hull",
                                Vector(0.0,0.0),0.0,Vector(0.0,0.0),0);

            hull-=effective_damage;

            if(!is_alive()){
                Game::create_explosion("explosion_ship","explosion_ship",Coords<double>(box.center_x(),box.center_y()),Game_Constants::EXPLOSION_DAMAGE_SHIP,get_faction());

                uint32_t items_to_drop=rng.random_range(get_ship_type()->item_drop_min,get_ship_type()->item_drop_max);

                for(uint32_t i=0;i<items_to_drop;i++){
                    Game::create_item(Coords<double>(box.center_x(),box.center_y()),velocity);
                }

                if(is_player){
                    Game::game_over();
                }
                else{
                    if(damage_faction=="player"){
                        Game::increase_score(get_ship_type()->point_value);
                    }
                }
            }
        }
    }
}

void Ship::set_thrust_angle(string direction){
    if(direction!="none"){
        if(direction=="left"){
            angle=180.0;
        }
        else if(direction=="right"){
            angle=0.0;
        }
        else if(direction=="up"){
            angle=90.0;
        }
        else if(direction=="down"){
            angle=270.0;
        }
        else if(direction=="left_up"){
            angle=135.0;
        }
        else if(direction=="right_up"){
            angle=45.0;
        }
        else if(direction=="left_down"){
            angle=225.0;
        }
        else if(direction=="right_down"){
            angle=315.0;
        }

        thrusting=true;
    }
    else{
        thrusting=false;
    }
}

void Ship::set_braking(bool new_braking){
    braking=new_braking;
}

void Ship::thrust(uint32_t frame){
    if(thrusting){
        Vector thrust_force(get_thrust_accel(),angle);

        force+=thrust_force;

        if(frame%15==0){
            Sound_Manager::play_sound("thrust",box.center_x(),box.center_y());
        }
    }
}

void Ship::brake(uint32_t frame){
    if(braking){
        Vector brake_force(get_thrust_decel(),velocity.direction+180.0);

        Math::clamp_angle(brake_force.direction);

        if(brake_force.magnitude/get_ship_type()->mass>velocity.magnitude){
            brake_force.magnitude=velocity.magnitude*get_ship_type()->mass;
        }

        force+=brake_force;

        if(frame%30==0){
            Sound_Manager::play_sound("brake",box.center_x(),box.center_y());
        }
    }
}

void Ship::stop(){
    velocity*=0.0;
    force*=0.0;
}

void Ship::commence_landing(uint32_t new_landing_planet_index){
    landing=true;
    landing_scale=1.0;
    landing_planet_index=new_landing_planet_index;

    velocity*=0.0;
}

bool Ship::is_landing() const{
    return landing;
}

void Ship::land(bool is_player){
    if(is_alive()){
        if(landing){
            landing_scale-=1.0/(double)Engine::UPDATE_RATE;

            if(landing_scale<=0.0){
                if(is_player){
                    Game::land(landing_planet_index);

                    landing=false;
                    landing_scale=1.0;
                    landing_planet_index=0;
                }
                else{
                    hull=0;
                }
            }
        }
    }
}

void Ship::regenerate_shields(bool is_player){
    if(is_alive() && !is_disabled(is_player)){
        if(shields<get_shields_max() && ++shield_recharge>=get_shield_recharge_rate()*Engine::UPDATE_RATE/1000){
            shield_recharge=0;

            shields++;
        }
    }
}

void Ship::cooldown(const Quadtree<double,uint32_t>& quadtree_ships,RNG& rng,uint32_t own_index){
    bool is_player=own_index==0;

    if(is_alive()){
        if(disabled_cooldown>0){
            disabled_cooldown--;
        }

        if(has_weapon()){
            uint32_t weapon_cool_point=Game_Data::get_upgrade_type(get_weapon_name())->cooldown*Engine::UPDATE_RATE/1000;

            if(weapon_cooldown<weapon_cool_point){
                weapon_cooldown++;
            }

            if(weapons_enabled && !is_disabled(is_player) && weapon_cooldown>=weapon_cool_point){
                if(fire_weapon(quadtree_ships,rng,own_index)){
                    weapon_cooldown=0;
                }
            }
        }
    }
}

bool Ship::faction_is_valid(string faction,bool weapon_check) const{
    if(get_faction()=="player"){
        return true;
    }
    else if(get_faction()=="civilian"){
        if(faction=="pirate" || (faction=="player" && !Game::is_player_tractored() && (Game::notoriety_tier_1() || Game::notoriety_tier_2()))){
            return true;
        }
        else{
            return false;
        }
    }
    else if(get_faction()=="pirate"){
        if(faction=="civilian" || faction=="police" || (faction=="player" && !Game::is_player_tractored())){
            return true;
        }
        else{
            return false;
        }
    }
    else if(get_faction()=="bounty_hunter"){
        if(faction=="player" && !Game::is_player_tractored()){
            return true;
        }
        else{
            return false;
        }
    }
    else if(get_faction()=="police"){
        if(faction=="pirate"){
            return true;
        }
        else if(faction=="player" && !Game::is_player_tractored()){
            if(!weapon_check && (Game::notoriety_tier_1() || Game::notoriety_tier_2())){
                return true;
            }
            else if(weapon_check && Game::notoriety_tier_2()){
                return true;
            }
            else{
                return false;
            }
        }
        else{
            return false;
        }
    }
    else{
        return false;
    }
}

int32_t Ship::get_nearest_valid_target_ship(const Quadtree<double,uint32_t>& quadtree_ships,uint32_t own_index,const Collision_Rect<double>& box_targeting,bool weapon_check){
    vector<uint32_t> nearby_ships;
    quadtree_ships.get_objects(nearby_ships,box_targeting);

    vector<uint32_t> valid_targets;

    unordered_set<uint32_t> collisions;

    //Find all valid targets
    for(size_t i=0;i<nearby_ships.size();i++){
        if(!collisions.count(nearby_ships[i])){
            collisions.emplace(nearby_ships[i]);

            const Ship& ship=Game::get_ship(nearby_ships[i]);

            if(ship.is_alive() && own_index!=nearby_ships[i] && !ship.is_landing() && (nearby_ships[i]!=0 || !Game::player_is_landed())){
                Collision_Rect<double> box_ship=ship.get_box();

                if(faction_is_valid(ship.get_faction(),weapon_check) && Collision::check_rect(box_targeting,box_ship)){
                    valid_targets.push_back(nearby_ships[i]);
                }
            }
        }
    }

    //Find nearest valid target
    int32_t nearest_index=-1;
    double nearest_distance=0.0;

    for(size_t i=0;i<valid_targets.size();i++){
        const Ship& ship=Game::get_ship(valid_targets[i]);

        double new_distance=Math::distance_between_points(box.center_x(),box.center_y(),ship.get_box().center_x(),ship.get_box().center_y());

        if(nearest_index==-1 || new_distance<nearest_distance){
            nearest_index=valid_targets[i];
            nearest_distance=new_distance;
        }
    }

    return nearest_index;
}

bool Ship::fire_weapon(const Quadtree<double,uint32_t>& quadtree_ships,RNG& rng,uint32_t own_index){
    if(has_weapon()){
        Collision_Rect<double> box_targeting=box;

        Upgrade* upgrade=Game_Data::get_upgrade_type(get_weapon_name());

        box_targeting.x-=upgrade->range;
        box_targeting.y-=upgrade->range;
        box_targeting.w+=upgrade->range*2.0;
        box_targeting.h+=upgrade->range*2.0;

        int32_t nearest_index=get_nearest_valid_target_ship(quadtree_ships,own_index,box_targeting,true);

        //If a nearest valid target was found
        if(nearest_index>=0){
            const Ship& ship=Game::get_ship((uint32_t)nearest_index);

            for(uint32_t i=0;i<upgrade->shots;i++){
                double angle_variation=0.0;

                if(upgrade->spreads_normally()){
                    if(rng.random_range(0,99)<50){
                        angle_variation+=rng.random_range(0,Game_Constants::WEAPON_SPREAD_DISTANCE);
                    }
                    else{
                        angle_variation-=rng.random_range(0,Game_Constants::WEAPON_SPREAD_DISTANCE);
                    }
                }
                else if(upgrade->spreads_evenly()){
                    angle_variation=(360.0/(double)upgrade->shots)*(double)i;
                }

                Game::create_shot(own_index,upgrade->shot_type,get_faction(),upgrade->name,Coords<double>(box.center_x(),box.center_y()),box.get_angle_to_rect(ship.get_box())+angle_variation);
            }

            if(upgrade->sound.length()>0){
                Sound_Manager::play_sound(upgrade->sound,box.center_x(),box.center_y());
            }

            return true;
        }
    }

    return false;
}

void Ship::ai_select_target(uint32_t own_index,RNG& rng){
    if(get_faction()=="civilian"){
        uint32_t nearest_planet_index=Game::get_nearest_planet(own_index);

        const Planet& planet=Game::get_planet(nearest_planet_index);

        ai_target=planet.get_circle().get_center();
    }
    else if(get_faction()=="pirate" || get_faction()=="bounty_hunter"){
        ai_target.x=(double)rng.random_range(0,(uint32_t)(Game::world_width-1.0));
        ai_target.y=(double)rng.random_range(0,(uint32_t)(Game::world_height-1.0));
    }
    else if(get_faction()=="police"){
        ai_target=ai_target_next;

        Vector select_target(Game_Constants::AI_PATROL_DISTANCE,rng.random_range(0,359));
        Vector_Components vc=select_target.get_components();
        ai_target.x+=vc.a;
        ai_target.y+=vc.b;
    }
}

void Ship::ai_check_for_proximity_target(const Quadtree<double,uint32_t>& quadtree_ships,uint32_t own_index){
    Collision_Rect<double> box_targeting=box;

    box_targeting.x-=Game_Constants::AI_RANGE;
    box_targeting.y-=Game_Constants::AI_RANGE;
    box_targeting.w+=Game_Constants::AI_RANGE*2.0;
    box_targeting.h+=Game_Constants::AI_RANGE*2.0;

    int32_t nearest_index=get_nearest_valid_target_ship(quadtree_ships,own_index,box_targeting,false);

    //If a nearest valid target was found
    if(nearest_index>=0){
        ai_has_proximity_target=true;
        ai_proximity_target=(uint32_t)nearest_index;

        const Ship& ship=Game::get_ship((uint32_t)nearest_index);

        string faction=ship.get_faction();

        ai_proximity_target_flee=false;

        if(get_faction()=="civilian"){
            if(faction=="pirate"){
                ai_proximity_target_flee=true;
            }
            else if(faction=="player" && (Game::notoriety_tier_1() || Game::notoriety_tier_2())){
                ai_proximity_target_flee=true;
            }
        }
        else if(get_faction()=="pirate"){
            if(faction=="police"){
                ai_proximity_target_flee=true;
            }
            else if(faction=="player" && Game::notoriety_tier_2()){
                ai_proximity_target_flee=true;
            }
        }
    }
    else{
        clear_proximity_target();
    }
}

bool Ship::ai_proximity_check_allowed(uint32_t frame,uint32_t own_index) const{
    if((frame+(own_index%Engine::UPDATE_RATE))%Game_Constants::AI_PROXIMITY_CHECK_PERIOD==0){
        return true;
    }
    else{
        return false;
    }
}

void Ship::ai(const Quadtree<double,uint32_t>& quadtree_ships,const Quadtree<double,uint32_t>& quadtree_planets,uint32_t frame,uint32_t own_index,RNG& rng){
    if(is_alive() && !is_landing() && !is_disabled(false)){
        if(ai_proximity_check_allowed(frame,own_index)){
            ai_check_for_proximity_target(quadtree_ships,own_index);
        }

        thrusting=false;
        set_braking(false);

        if(ai_has_proximity_target){
            angle=Math::get_angle_to_point(box.get_center(),Game::get_ship(ai_proximity_target).box.get_center());

            if(ai_proximity_target_flee){
                angle+=180.0;
            }

            if(get_faction()=="police" && ai_proximity_target==0 && Game::notoriety_tier_1() && get_distance_to_player()<=Game_Constants::TRACTOR_RANGE){
                Game::tractor_player(angle+180.0,own_index);

                set_braking(true);
            }
            else{
                if(ai_proximity_target_flee || get_distance_to_proximity_target()>Game_Constants::AI_FOLLOW_DISTANCE){
                    thrusting=true;
                }
            }
        }
        else{
            if(get_faction()=="police"){
                if(Collision::check_rect(box,Collision_Rect<double>(ai_target.x,ai_target.y,1.0,1.0))){
                    swap(ai_target,ai_target_next);
                }
            }
            else{
                if(get_faction()=="civilian"){
                    vector<uint32_t> nearby_planets;
                    quadtree_planets.get_objects(nearby_planets,box);

                    unordered_set<uint32_t> collisions;

                    for(size_t i=0;i<nearby_planets.size();i++){
                        if(!collisions.count(nearby_planets[i])){
                            collisions.emplace(nearby_planets[i]);

                            const Planet& planet=Game::get_planet(nearby_planets[i]);

                            if(Collision::check_rect_circ(box,planet.get_circle())){
                                commence_landing(nearby_planets[i]);

                                break;
                            }
                        }
                    }
                }
                else if(get_faction()=="pirate" || get_faction()=="bounty_hunter"){
                    if(Collision::check_rect(box,Collision_Rect<double>(ai_target.x,ai_target.y,1.0,1.0))){
                        ai_select_target(own_index,rng);
                    }
                }
            }

            angle=Math::get_angle_to_point(box.get_center(),ai_target);
            thrusting=true;
        }
    }
}

void Ship::accelerate(bool is_player,uint32_t frame){
    if(is_alive() && !is_landing() && (!is_player || !Game::player_is_landed())){
        thrust(frame);
        brake(frame);

        if(is_player && Game::is_player_tractored()){
            apply_tractor(Game::get_tractor_angle());
        }

        Vector acceleration=force/get_ship_type()->mass;

        velocity+=acceleration;

        if(velocity.magnitude>get_max_speed()){
            velocity.magnitude=get_max_speed();
        }

        force*=0.0;
    }
}

void Ship::movement(uint32_t own_index,const Quadtree<double,uint32_t>& quadtree_debris,const Quadtree<double,uint32_t>& quadtree_shots,
                    const Quadtree<double,uint32_t>& quadtree_explosions,const Quadtree<double,uint32_t>& quadtree_items,RNG& rng){
    bool is_player=own_index==0;

    if(is_alive() && !is_landing() && (!is_player || !Game::player_is_landed())){
        Vector_Components vc=velocity.get_components();

        box.x+=vc.a/(double)Engine::UPDATE_RATE;
        box.y+=vc.b/(double)Engine::UPDATE_RATE;

        Collision_Rect<double> box_collision=get_collision_box();

        vector<uint32_t> nearby_debris;
        quadtree_debris.get_objects(nearby_debris,box_collision);

        unordered_set<uint32_t> collisions;

        for(size_t i=0;i<nearby_debris.size();i++){
            if(is_alive()){
                if(!collisions.count(nearby_debris[i])){
                    collisions.emplace(nearby_debris[i]);

                    const Debris& debris=Game::get_debris(nearby_debris[i]);
                    Collision_Rect<double> box_debris=debris.get_collision_box();

                    if(rng.random_range(0,99)<Game_Constants::COLLISION_CHANCE_DEBRIS && Collision::check_rect_rotated(box_collision,box_debris,angle,debris.get_angle())){
                        take_damage(is_player,debris.get_debris_type()->damage,debris.get_debris_type()->damage_type,
                                    Coords<double>(box_collision.x+rng.random_range(0,(uint32_t)box_collision.w),box_collision.y+rng.random_range(0,(uint32_t)box_collision.h)),"world",rng);
                    }
                }
            }
            else{
                return;
            }
        }

        vector<uint32_t> nearby_shots;
        quadtree_shots.get_objects(nearby_shots,box_collision);

        collisions.clear();

        for(size_t i=0;i<nearby_shots.size();i++){
            if(is_alive()){
                if(!collisions.count(nearby_shots[i])){
                    collisions.emplace(nearby_shots[i]);

                    const Shot& shot=Game::get_shot(nearby_shots[i]);
                    Collision_Rect<double> box_shot=shot.get_collision_box();

                    if(shot.is_alive() && (!shot.has_owner() || own_index!=shot.get_owner_index()) && Collision::check_rect_rotated(box_collision,box_shot,angle,shot.get_angle())){
                        if(shot.get_shot_type()->damage_type=="explosive"){
                            Game::create_explosion("explosion_missile","explosion_missile",Coords<double>(box_shot.center_x(),box_shot.center_y()),shot.get_firing_upgrade()->damage,shot.get_faction());
                        }
                        else{
                            take_damage(is_player,shot.get_firing_upgrade()->damage,shot.get_shot_type()->damage_type,
                                        Coords<double>(box_collision.x+rng.random_range(0,(uint32_t)box_collision.w),box_collision.y+rng.random_range(0,(uint32_t)box_collision.h)),shot.get_faction(),rng);
                        }

                        Game::kill_shot(nearby_shots[i]);
                    }
                }
            }
            else{
                return;
            }
        }

        vector<uint32_t> nearby_explosions;
        quadtree_explosions.get_objects(nearby_explosions,box_collision);

        collisions.clear();

        for(size_t i=0;i<nearby_explosions.size();i++){
            if(is_alive()){
                if(!collisions.count(nearby_explosions[i])){
                    collisions.emplace(nearby_explosions[i]);

                    const Explosion& explosion=Game::get_explosion(nearby_explosions[i]);
                    Collision_Circ<double> circle_explosion=explosion.get_circle();

                    if(explosion.is_alive() && !was_damaged_by_explosion(nearby_explosions[i]) && Collision::check_circ_rect(circle_explosion,box_collision)){
                        damaged_by_explosion(nearby_explosions[i]);

                        Collision_Rect<double> box_area=Collision::get_collision_area_rect(box_collision,Collision_Rect<double>(circle_explosion.x-circle_explosion.r,
                                                                                                                                circle_explosion.y-circle_explosion.r,
                                                                                                                                circle_explosion.r*2.0,circle_explosion.r*2.0));

                        take_damage(is_player,explosion.get_damage(),"explosive",
                                    Coords<double>(box_area.x+rng.random_range(0,(uint32_t)box_area.w),box_area.y+rng.random_range(0,(uint32_t)box_area.h)),explosion.get_faction(),rng);
                    }
                }
            }
            else{
                return;
            }
        }

        if(is_player){
            vector<uint32_t> nearby_items;
            quadtree_items.get_objects(nearby_items,box);

            collisions.clear();

            for(size_t i=0;i<nearby_items.size();i++){
                if(!collisions.count(nearby_items[i])){
                    collisions.emplace(nearby_items[i]);

                    const Item& item=Game::get_item(nearby_items[i]);

                    if(item.is_alive() && can_use_item(item.get_item_type()) && Collision::check_rect(item.get_box(),box)){
                        use_item(item.get_item_type());

                        item.play_collection_sound();

                        Game::kill_item(nearby_items[i]);
                    }
                }
            }

            if(Game::is_player_tractored()){
                const Ship& ship=Game::get_ship(Game::get_tractoring_ship_index());

                if(Collision::check_rect_rotated(box_collision,ship.get_collision_box(),angle,ship.get_angle())){
                    Game::arrest_player();
                }
            }
        }

        if(box.x<0.0){
            box.x=0.0;
        }
        if(box.y<0.0){
            box.y=0.0;
        }
        if(box.x+box.w>=Game::world_width){
            box.x=Game::world_width-box.w;
        }
        if(box.y+box.h>=Game::world_height){
            box.y=Game::world_height-box.h;
        }
    }
}

void Ship::animate(){
    if(is_alive()){
        if(thrusting){
            sprite.animate();
        }
        else{
            sprite.reset_animation();
        }
    }
}

void Ship::render(bool tractoring){
    if(is_alive()){
        if(Collision::check_rect(box*Game_Manager::camera_zoom,Game_Manager::camera)){
            double scale=1.0;
            if(is_landing()){
                scale=landing_scale;
            }

            sprite.render(box.x*Game_Manager::camera_zoom-Game_Manager::camera.x,box.y*Game_Manager::camera_zoom-Game_Manager::camera.y,1.0,scale,scale,angle);

            ///QQQ render collision boxes
            /**Collision_Rect<double> col_box=get_collision_box();
            ///Render::render_rectangle(box.x*Game_Manager::camera_zoom-Game_Manager::camera.x,box.y*Game_Manager::camera_zoom-Game_Manager::camera.y,box.w,box.h,0.25,"white");
            ///Render::render_rectangle(col_box.x*Game_Manager::camera_zoom-Game_Manager::camera.x,col_box.y*Game_Manager::camera_zoom-Game_Manager::camera.y,col_box.w,col_box.h,0.25,"red");

            vector<Coords<double>> vertices;
            col_box.get_vertices(vertices,angle);

            for(size_t i=0;i<vertices.size();i++){
                uint32_t start_vertex=(uint32_t)i;
                uint32_t end_vertex=(i<vertices.size()-1) ? start_vertex+1 : 0;

                Render::render_line(vertices[start_vertex].x*Game_Manager::camera_zoom-Game_Manager::camera.x,vertices[start_vertex].y*Game_Manager::camera_zoom-Game_Manager::camera.y,
                                    vertices[end_vertex].x*Game_Manager::camera_zoom-Game_Manager::camera.x,vertices[end_vertex].y*Game_Manager::camera_zoom-Game_Manager::camera.y,1.0,"red");
            }*/
            ///
        }

        if(tractoring){
            const Sprite& tractor_sprite=Game::get_tractor_sprite();

            double x=box.center_x()-tractor_sprite.get_width()/2.0;
            double y=box.center_y()-tractor_sprite.get_height()/2.0;

            Coords<double> box_side(box.x+box.w,box.center_y());
            box_side=Math::rotate_point(box_side,box.get_center(),angle);

            Vector move_sprite(Math::distance_between_points(box.center_x(),box.center_y(),box_side.x,box_side.y),angle);
            Vector_Components vc=move_sprite.get_components();
            x+=vc.a;
            y+=vc.b;

            tractor_sprite.render(x*Game_Manager::camera_zoom-Game_Manager::camera.x,y*Game_Manager::camera_zoom-Game_Manager::camera.y,1.0,1.0,1.0,angle);
        }
    }
}
