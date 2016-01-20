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
///QQQ includes
///#include <render.h>
///

#include <unordered_set>

using namespace std;

Ship::Ship(string new_type,const Coords<double>& position,double new_angle,const vector<string>& starting_upgrades){
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

    weapons_enabled=false;
    weapon_cooldown=0;

    sprite.set_name(get_ship_type()->sprite);

    box.w=sprite.get_width();
    box.h=sprite.get_height();

    for(size_t i=0;i<starting_upgrades.size();i++){
        add_upgrade(starting_upgrades[i]);
    }
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

void Ship::take_damage(bool is_player,int32_t damage,string damage_type,const Coords<double>& location){
    if(is_alive()){
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

                Game::create_effect("explosion_shields",scale,location,"explosion_shields",
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

            Game::create_effect("explosion_hull",scale,location,"explosion_hull",
                                Vector(0.0,0.0),0.0,Vector(0.0,0.0),0);

            hull-=effective_damage;

            if(!is_alive()){
                ///QQQ create explosion

                if(is_player){
                    Game::game_over();
                }
                else{
                    Game::increase_score(get_ship_type()->point_value);
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

void Ship::commence_landing(uint32_t new_landing_planet_index){
    landing=true;
    landing_scale=1.0;
    landing_planet_index=new_landing_planet_index;

    velocity*=0.0;
}

bool Ship::is_landing(){
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

void Ship::regenerate_shields(){
    if(is_alive()){
        if(shields<get_shields_max() && ++shield_recharge>=get_shield_recharge_rate()*Engine::UPDATE_RATE/1000){
            shield_recharge=0;

            shields++;
        }
    }
}

void Ship::cooldown(const Quadtree<double,uint32_t>& quadtree_ships,RNG& rng,uint32_t own_index){
    if(is_alive()){
        if(has_weapon()){
            uint32_t weapon_cool_point=Game_Data::get_upgrade_type(get_weapon_name())->cooldown*Engine::UPDATE_RATE/1000;

            if(weapon_cooldown<weapon_cool_point){
                weapon_cooldown++;
            }

            if(weapons_enabled && weapon_cooldown>=weapon_cool_point){
                if(fire_weapon(quadtree_ships,rng,own_index)){
                    weapon_cooldown=0;
                }
            }
        }
    }
}

bool Ship::fire_weapon(const Quadtree<double,uint32_t>& quadtree_ships,RNG& rng,uint32_t own_index){
    if(has_weapon()){
        Upgrade* upgrade=Game_Data::get_upgrade_type(get_weapon_name());

        Collision_Rect<double> box_targeting=box;

        box_targeting.x-=upgrade->range;
        box_targeting.y-=upgrade->range;
        box_targeting.w+=upgrade->range*2.0;
        box_targeting.h+=upgrade->range*2.0;

        vector<uint32_t> nearby_ships;
        quadtree_ships.get_objects(nearby_ships,box_targeting);

        vector<uint32_t> valid_targets;

        unordered_set<uint32_t> collisions;

        //Find all valid targets
        for(size_t i=0;i<nearby_ships.size();i++){
            if(!collisions.count(nearby_ships[i])){
                collisions.emplace(nearby_ships[i]);

                if(own_index!=nearby_ships[i]){
                    const Ship& ship=Game::get_ship(nearby_ships[i]);
                    Collision_Rect<double> box_ship=ship.get_box();

                    ///QQQ add faction check
                    if(Collision::check_rect(box_targeting,box_ship)){
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

                Game::create_shot(own_index,upgrade->shot_type,upgrade->name,Coords<double>(box.center_x(),box.center_y()),Collision::get_angle_to_rect(box,ship.get_box())+angle_variation);
            }

            if(upgrade->sound.length()>0){
                Sound_Manager::play_sound(upgrade->sound,box.center_x(),box.center_y());
            }

            return true;
        }
    }

    return false;
}

void Ship::accelerate(bool is_player,uint32_t frame){
    if(is_alive() && !is_landing() && (!is_player || !Game::player_is_landed())){
        thrust(frame);
        brake(frame);

        Vector acceleration=force/get_ship_type()->mass;

        velocity+=acceleration;

        if(velocity.magnitude>get_max_speed()){
            velocity.magnitude=get_max_speed();
        }
        else if(velocity.magnitude<-get_max_speed()){
            velocity.magnitude=-get_max_speed();
        }

        force*=0.0;
    }
}

void Ship::movement(uint32_t own_index,const Quadtree<double,uint32_t>& quadtree_debris,const Quadtree<double,uint32_t>& quadtree_shots,RNG& rng){
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
            if(!collisions.count(nearby_debris[i])){
                collisions.emplace(nearby_debris[i]);

                const Debris& debris=Game::get_debris(nearby_debris[i]);
                Collision_Rect<double> box_debris=debris.get_collision_box();

                if(rng.random_range(0,99)<Game_Constants::COLLISION_CHANCE_DEBRIS && Collision::check_rect(box_collision,box_debris)){
                    Collision_Rect<double> box_area=Collision::get_collision_area_rect(box_collision,box_debris);

                    take_damage(is_player,debris.get_debris_type()->damage,debris.get_debris_type()->damage_type,Coords<double>(box_area.center_x(),box_area.center_y()));
                }
            }
        }

        vector<uint32_t> nearby_shots;
        quadtree_shots.get_objects(nearby_shots,box_collision);

        collisions.clear();

        for(size_t i=0;i<nearby_shots.size();i++){
            if(!collisions.count(nearby_shots[i])){
                collisions.emplace(nearby_shots[i]);

                const Shot& shot=Game::get_shot(nearby_shots[i]);
                Collision_Rect<double> box_shot=shot.get_collision_box();

                if((!shot.has_owner() || own_index!=shot.get_owner_index()) && Collision::check_rect(box_collision,box_shot)){
                    Collision_Rect<double> box_area=Collision::get_collision_area_rect(box_collision,box_shot);

                    take_damage(is_player,shot.get_firing_upgrade()->damage,shot.get_shot_type()->damage_type,Coords<double>(box_area.center_x(),box_area.center_y()));

                    Game::kill_shot(nearby_shots[i]);
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

void Ship::render(){
    if(is_alive()){
        if(Collision::check_rect(box*Game_Manager::camera_zoom,Game_Manager::camera)){
            double scale=1.0;
            if(is_landing()){
                scale=landing_scale;
            }

            sprite.render(box.x*Game_Manager::camera_zoom-Game_Manager::camera.x,box.y*Game_Manager::camera_zoom-Game_Manager::camera.y,1.0,scale,scale,angle);

            ///QQQ render collision boxes
            /**Collision_Rect<double> col_box=get_collision_box();
            Render::render_rectangle(box.x*Game_Manager::camera_zoom-Game_Manager::camera.x,box.y*Game_Manager::camera_zoom-Game_Manager::camera.y,box.w,box.h,0.5,"ui_white");
            Render::render_rectangle(col_box.x*Game_Manager::camera_zoom-Game_Manager::camera.x,col_box.y*Game_Manager::camera_zoom-Game_Manager::camera.y,col_box.w,col_box.h,0.5,"red");*/
            ///
        }
    }
}
