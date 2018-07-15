/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "ship.h"
#include "game_data.h"
#include "game.h"
#include "game_constants.h"
#include "game_options.h"
#include "android_achievements.h"

#include <engine.h>
#include <game_manager.h>
#include <sound_manager.h>
#include <render.h>
#include <screen_shake.h>
#include <controller_manager.h>
#include <engine_math.h>
#include <engine_strings.h>

#include <unordered_set>

using namespace std;

Ship::Ship(string new_type,const Coords<double>& position,double new_angle){
    type=new_type;

    box.x=position.x;
    box.y=position.y;

    angle=new_angle;
    render_angle = new_angle;

    hull=get_hull_max();
    shields=get_shields_max();

    thrusting=false;
    braking=false;

    landing=false;
    landing_scale=1.0;
    landing_planet_index=0;

    cast_shadow = false;

    shield_recharge=0;

    weapons_enabled=true;
    weapon_cooldown=0;

    active_cooldown=0;
    scanner_startup_sprite.set_name("scanner_startup");
    scanner_startup_sprite.animating=false;

    cloaked=false;
    warping=false;
    power_drain=0;

    laser_has_target=false;

    disabled_cooldown=0;

    point_defense_cooldown=0;

    in_processing_range=false;

    sound_cooldown_chasing_player=0;

    sprite.set_name(get_ship_type()->sprite);

    box.w=sprite.get_width();
    box.h=sprite.get_height();

    for(size_t i=0;i<get_ship_type()->upgrades.size();i++){
        add_upgrade(get_ship_type()->upgrades[i]);
    }

    reset_ai_ignore_angle_mods();

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

Vector Ship::get_angular_velocity () const {
    return get_ship_type()->angular_velocity;
}

int32_t Ship::get_hull() const{
    return hull;
}

int32_t Ship::get_shields() const{
    return shields;
}

uint32_t Ship::get_shield_recharge_rate() const{
    uint32_t value=Game_Constants::SHIELD_RECHARGE_RATE;

    //First, add all shield_recharge_rate_down's
    for(size_t i=0;i<upgrades.size();i++){
        Upgrade* upgrade=Game_Data::get_upgrade_type(upgrades[i]);

        if(upgrade->is_passive()){
            value+=upgrade->shield_recharge_rate_down;
        }
    }

    //Then, subtract all shield_recharge_rate_up's
    for(size_t i=0;i<upgrades.size();i++){
        Upgrade* upgrade=Game_Data::get_upgrade_type(upgrades[i]);

        if(upgrade->is_passive()){
            if(upgrade->shield_recharge_rate_up<=value){
                value-=upgrade->shield_recharge_rate_up;
            }
            else{
                value=0;

                break;
            }
        }
    }

    return value;
}

uint32_t Ship::get_cooldown(uint32_t cooldown_base) const{
    uint32_t value=cooldown_base;

    //First, add all cooldown_up's
    for(size_t i=0;i<upgrades.size();i++){
        Upgrade* upgrade=Game_Data::get_upgrade_type(upgrades[i]);

        if(upgrade->is_passive()){
            value+=upgrade->cooldown_up;
        }
    }

    //Then, subtract all cooldown_down's
    for(size_t i=0;i<upgrades.size();i++){
        Upgrade* upgrade=Game_Data::get_upgrade_type(upgrades[i]);

        if(upgrade->is_passive()){
            if(upgrade->cooldown_down<=value){
                value-=upgrade->cooldown_down;
            }
            else{
                value=0;

                break;
            }
        }
    }

    return value;
}

int32_t Ship::get_damage_mod_solid() const{
    int32_t value=0;

    for(size_t i=0;i<upgrades.size();i++){
        Upgrade* upgrade=Game_Data::get_upgrade_type(upgrades[i]);

        if(upgrade->is_passive()){
            value+=upgrade->solid_damage_mod;
        }
    }

    return value;
}

int32_t Ship::get_damage_mod_explosive() const{
    int32_t value=0;

    for(size_t i=0;i<upgrades.size();i++){
        Upgrade* upgrade=Game_Data::get_upgrade_type(upgrades[i]);

        if(upgrade->is_passive()){
            value+=upgrade->explosive_damage_mod;
        }
    }

    return value;
}

int32_t Ship::get_damage_mod_energy() const{
    int32_t value=0;

    for(size_t i=0;i<upgrades.size();i++){
        Upgrade* upgrade=Game_Data::get_upgrade_type(upgrades[i]);

        if(upgrade->is_passive()){
            value+=upgrade->energy_damage_mod;
        }
    }

    return value;
}

vector<string> Ship::get_upgrades() const{
    return upgrades;
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

uint32_t Ship::get_weapon_cooldown() const{
    return weapon_cooldown;
}

uint32_t Ship::get_active_cooldown() const{
    return active_cooldown;
}

void Ship::add_upgrade(string name){
    if(!has_upgrade(name)){
        Upgrade* upgrade=Game_Data::get_upgrade_type(name);

        if(upgrade->is_weapon()){
            remove_weapon();

            weapon_cooldown=0;
        }
        else if(upgrade->is_active()){
            remove_active();

            active_cooldown=0;
        }

        upgrades.push_back(name);

        clamp_stats();
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

    clamp_stats();
}

void Ship::clamp_stats(){
    if(hull>get_hull_max()){
        hull=get_hull_max();
    }

    if(shields>get_shields_max()){
        shields=get_shields_max();
    }
}

bool Ship::has_laser() const{
    return has_upgrade("plasma_gun");
}

void Ship::calculate_laser_target(const Quadtree<double,uint32_t>& quadtree_ships,uint32_t own_index){
    bool is_player=own_index==0;

    if(is_alive() && !is_landing() && (!is_player || !Game::player_is_landed()) && !is_disabled(is_player) && is_in_processing_range()){
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

bool Ship::has_point_defense() const{
    for(size_t i=0;i<upgrades.size();i++){
        if(Game_Data::get_upgrade_type(upgrades[i])->gives_point_defense()){
            return true;
        }
    }

    return false;
}

string Ship::get_point_defense_name() const{
    for(size_t i=0;i<upgrades.size();i++){
        if(Game_Data::get_upgrade_type(upgrades[i])->gives_point_defense()){
            return upgrades[i];
        }
    }

    return "";
}

bool Ship::is_cloaked() const{
    return cloaked;
}

void Ship::toggle_cloak(){
    cloaked=!cloaked;

    if(!cloaked){
        active_cooldown=0;
        weapon_cooldown=0;
        point_defense_cooldown=0;
    }

    power_drain=0;
}

bool Ship::is_warping() const{
    return warping;
}

void Ship::toggle_warp(){
    warping=!warping;

    if(!warping){
        active_cooldown=0;
    }

    power_drain=0;
}

bool Ship::is_disabled(bool is_player) const{
    return disabled_cooldown>0 || (is_player && Game::player_is_out_of_power());
}

void Ship::disable(){
    disabled_cooldown=Game_Constants::DISABLED_LENGTH*Engine::UPDATE_RATE;

    thrusting=false;
    braking=false;

    //Disable scanner startup
    if(scanner_startup_sprite.animating){
        scanner_startup_sprite.animating=false;
    }

    //Disable cloak
    if(is_cloaked()){
        toggle_cloak();
    }

    //Disable warp
    if(is_warping()){
        toggle_warp();
    }
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
        Game::increase_power_item();
    }

    Game::increase_score(item_type->point_value);
}

void Ship::apply_tractor(double force_angle){
    Vector tractor_force(Game_Constants::TRACTOR_FORCE,force_angle);

    force+=tractor_force;
}

double Ship::get_thrust_accel() const{
    if(!is_warping()){
        double value=get_ship_type()->thrust_accel;

        for(size_t i=0;i<upgrades.size();i++){
            Upgrade* upgrade=Game_Data::get_upgrade_type(upgrades[i]);

            if(upgrade->is_passive()){
                value+=upgrade->thrust_accel;
            }
        }

        value += Game::get_score_multiplier_accel();

        if(value<1.0){
            value=1.0;
        }

        return value;
    }
    else{
        return Game_Constants::WARP_ACCEL;
    }
}

double Ship::get_thrust_decel() const{
    if(!is_warping()){
        double value=get_ship_type()->thrust_decel;

        for(size_t i=0;i<upgrades.size();i++){
            Upgrade* upgrade=Game_Data::get_upgrade_type(upgrades[i]);

            if(upgrade->is_passive()){
                value+=upgrade->thrust_decel;
            }
        }

        value += Game::get_score_multiplier_decel();

        if(value<1.0){
            value=1.0;
        }

        return value;
    }
    else{
        return Game_Constants::WARP_DECEL;
    }
}

double Ship::get_max_speed() const{
    if(!is_warping()){
        double value=get_ship_type()->max_speed;

        for(size_t i=0;i<upgrades.size();i++){
            Upgrade* upgrade=Game_Data::get_upgrade_type(upgrades[i]);

            if(upgrade->is_passive()){
                value+=upgrade->max_speed;
            }
        }

        value += Game::get_score_multiplier_max_speed();

        if(value<1.0){
            value=1.0;
        }

        return value;
    }
    else{
        return Game_Constants::WARP_MAX_SPEED;
    }
}

int32_t Ship::get_hull_max() const{
    int32_t value=get_ship_type()->hull_max;

    for(size_t i=0;i<upgrades.size();i++){
        Upgrade* upgrade=Game_Data::get_upgrade_type(upgrades[i]);

        if(upgrade->is_passive()){
            value+=upgrade->max_hull;
        }
    }

    if(value<1){
        value=1;
    }

    return value;
}

int32_t Ship::get_shields_max() const{
    int32_t value=get_ship_type()->shields_max;

    for(size_t i=0;i<upgrades.size();i++){
        Upgrade* upgrade=Game_Data::get_upgrade_type(upgrades[i]);

        if(upgrade->is_passive()){
            value+=upgrade->max_shields;
        }
    }

    if(value<0){
        value=0;
    }

    return value;
}

double Ship::get_mass() const{
    return get_ship_type()->mass;
}

string Ship::get_faction() const{
    return get_ship_type()->faction;
}

Collision_Rect<double> Ship::get_collision_box() const{
    return get_ship_type()->box_collision;
}

vector<Coords<double>> Ship::get_collision_vertices() const {
    vector<Coords<double>> vertices_box;
    box.get_vertices(vertices_box, angle);

    vector<Coords<double>> vertices_collision;
    get_collision_box().get_vertices(vertices_collision, angle);

    double translate_x = vertices_collision[Collision::VERTEX_UPPER_LEFT].x - vertices_box[Collision::VERTEX_UPPER_LEFT].x;
    double translate_y = vertices_collision[Collision::VERTEX_UPPER_LEFT].y - vertices_box[Collision::VERTEX_UPPER_LEFT].y;

    for (size_t i = 0; i < vertices_collision.size(); i++) {
        vertices_collision[i].x -= translate_x;
        vertices_collision[i].y -= translate_y;
    }

    double angle_x = Math::get_angle_to_point(vertices_box[Collision::VERTEX_UPPER_LEFT], vertices_box[Collision::VERTEX_UPPER_RIGHT]);
    double angle_y = Math::get_angle_to_point(vertices_box[Collision::VERTEX_UPPER_LEFT], vertices_box[Collision::VERTEX_LOWER_RIGHT]);

    Vector_Components vc_x = Vector(get_collision_box().x, angle_x).get_components();
    for (size_t i = 0; i < vertices_collision.size(); i++) {
        vertices_collision[i].x += vc_x.a;
        vertices_collision[i].y += vc_x.b;
    }

    Vector_Components vc_y = Vector(get_collision_box().y, angle_y).get_components();
    for (size_t i = 0; i < vertices_collision.size(); i++) {
        vertices_collision[i].x += vc_y.a;
        vertices_collision[i].y += vc_y.b;
    }

    return vertices_collision;
}

bool Ship::is_alive() const{
    return hull>0;
}

void Ship::check_processing_range(bool is_player){
    in_processing_range=(is_player || get_distance_to_player()<=Game_Constants::NPC_PROCESS_RANGE);
}

bool Ship::is_in_processing_range() const{
    return in_processing_range;
}

double Ship::get_distance_to_player() const{
    const Ship& player=Game::get_player_const();

    return Math::get_distance_between_points(box.get_center(),player.get_box().get_center());
}

double Ship::get_distance_to_proximity_target() const{
    if(ai_has_proximity_target){
        const Ship& ship=Game::get_ship(ai_proximity_target);

        return Math::get_distance_between_points(box.get_center(),ship.get_box().get_center());
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

void Ship::reset_ai_ignore_angle_mods(){
    time_without_moving=0;
    ai_ignore_angle_mods=false;
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

void Ship::die(bool is_player,string damage_faction,RNG& rng){
    if(is_alive()){
        if(damage_faction=="player" && !is_player && (get_faction()=="civilian" || get_faction()=="police")){
            Game::increase_notoriety(Game_Constants::NOTORIETY_INCREASE_DAMAGE);
        }

        //Disable cloak
        if(is_cloaked()){
            toggle_cloak();
        }

        hull=0;

        Game::create_explosion("explosion_ship","explosion_ship",Coords<double>(box.center_x(),box.center_y()),Game_Constants::EXPLOSION_DAMAGE_SHIP,damage_faction);

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

                Game::add_kill(get_faction());
            }
        }
    }
}

void Ship::take_damage(bool is_player,int32_t damage,string damage_type,const Coords<double>& location,string damage_faction,RNG& rng){
    if(is_alive()){
        if(damage_faction=="player" && !is_player && (get_faction()=="civilian" || get_faction()=="police")){
            Game::increase_notoriety(Game_Constants::NOTORIETY_INCREASE_DAMAGE);
        }

        //Disable cloak
        if(is_cloaked()){
            toggle_cloak();
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
                Game::create_effect("effect_explosion_shields",false,1.0,location,"effect_explosion_shields",
                                    Vector(0.0,0.0),0.0,Vector(0.0,0.0),0,false,Coords<double>());

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
            Game::create_effect("effect_hull_damage",true,0.1*(double)rng.random_range(1,10),location,"effect_hull_damage",
                                Vector(rng.random_range(0,10),rng.random_range(0,359)),rng.random_range(0,359),
                                Vector(0.01*rng.random_range(0,50),rng.random_range(0,359)),Game_Constants::EFFECT_LENGTH_HULL_DAMAGE,false,Coords<double>(),get_ship_type()->color);

            if (rng.random_range(0,99)<50) {
                Coords<double> smoke_location = location;
                if (rng.random_range(0,1)) {
                    smoke_location.x -= rng.random_range(1,10);
                } else {
                    smoke_location.x += rng.random_range(1,10);
                }
                if (rng.random_range(0,1)) {
                    smoke_location.y -= rng.random_range(1,10);
                } else {
                    smoke_location.y += rng.random_range(1,10);
                }

                Game::create_effect("effect_smoke_"+Strings::num_to_string(rng.random_range(0,2)),true,0.1*(double)rng.random_range(5,15),smoke_location,"",
                                    Vector(rng.random_range(0,10),rng.random_range(0,359)),rng.random_range(0,359),
                                    Vector(0.01*rng.random_range(0,50),rng.random_range(0,359)),Game_Constants::EFFECT_LENGTH_SMOKE,false,Coords<double>(),"white");
            }

            if (is_player) {
                Screen_Shake::add_shake(Game_Constants::SHAKE_MAGNITUDE_DAMAGE, Game_Constants::SHAKE_LENGTH_DAMAGE);
                Controller_Manager::make_rumble(Controller_Manager::CONTROLLER_ID_ALL, Game_Constants::SHAKE_MAGNITUDE_DAMAGE, Game_Constants::SHAKE_LENGTH_DAMAGE);
            }

            hull-=effective_damage;

            if(!is_alive()){
                Game::create_explosion("explosion_ship","explosion_ship",Coords<double>(box.center_x(),box.center_y()),Game_Constants::EXPLOSION_DAMAGE_SHIP,damage_faction);

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

                        Game::add_kill(get_faction());
                    }
                }
            }
        }
    }
}

void Ship::set_thrusting(bool new_thrusting){
    thrusting=new_thrusting;
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
    if(is_alive() && is_in_processing_range()){
        if(landing){
            landing_scale-=Game_Constants::SHIP_LANDING_RATE;

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
    if(is_alive() && !is_disabled(is_player) && is_in_processing_range()){
        if(shields<get_shields_max() && ++shield_recharge>=get_shield_recharge_rate()*Engine::UPDATE_RATE/1000){
            shield_recharge=0;

            shields++;
        }
    }
}

void Ship::drain_power(bool is_player){
    if(is_player){
        if(is_cloaked() || is_warping()){
            if(++power_drain>=Game_Constants::ACTIVE_POWER_DRAIN_RATE*Engine::UPDATE_RATE/1000){
                Game::use_power(Game_Data::get_upgrade_type(get_active_name())->power_use*Engine::UPDATE_RATE);

                power_drain=0;
            }
        }
    }
}

void Ship::cooldown(const Quadtree<double,uint32_t>& quadtree_ships,const Quadtree<double,uint32_t>& quadtree_shots,RNG& rng,uint32_t own_index){
    bool is_player=own_index==0;

    if(is_alive() && is_in_processing_range()){
        if(disabled_cooldown>0){
            disabled_cooldown--;
        }

        if(has_weapon()){
            uint32_t cool_point=get_cooldown(Game_Data::get_upgrade_type(get_weapon_name())->cooldown)*Engine::UPDATE_RATE/1000;

            if(weapon_cooldown<cool_point){
                weapon_cooldown++;
            }

            if(weapons_enabled && !is_disabled(is_player) && !is_cloaked() && !is_warping() && weapon_cooldown>=cool_point){
                if(fire_weapon(quadtree_ships,rng,own_index)){
                    weapon_cooldown=0;
                }
            }
        }

        if(has_active()){
            uint32_t cool_point=get_cooldown(Game_Data::get_upgrade_type(get_active_name())->cooldown)*Engine::UPDATE_RATE/1000;

            if(active_cooldown<cool_point){
                active_cooldown++;
            }
        }

        if(has_point_defense()){
            uint32_t cool_point=get_cooldown(Game_Data::get_upgrade_type(get_point_defense_name())->cooldown)*Engine::UPDATE_RATE/1000;

            if(point_defense_cooldown<cool_point){
                point_defense_cooldown++;
            }

            if(!is_disabled(is_player) && !is_cloaked() && !is_warping() && point_defense_cooldown>=cool_point){
                if(fire_point_defense(quadtree_shots)){
                    point_defense_cooldown=0;
                }
            }
        }
    }
}

bool Ship::faction_is_valid(string faction,bool weapon_check) const{
    if(get_faction()=="player"){
        if(faction!="player"){
            return true;
        }
    }
    else if(get_faction()=="civilian"){
        if(faction=="pirate" || (faction=="player" && !Game::is_player_tractored() && (Game::notoriety_tier_1() || Game::notoriety_tier_2()))){
            return true;
        }
    }
    else if(get_faction()=="pirate"){
        if(faction=="civilian" || faction=="police" || (faction=="player" && !Game::is_player_tractored())){
            return true;
        }
    }
    else if(get_faction()=="bounty_hunter"){
        if(faction=="player" && !Game::is_player_tractored()){
            return true;
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
        }
    }

    return false;
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

            if(ship.is_alive() && own_index!=nearby_ships[i] && !ship.is_landing() && (nearby_ships[i]!=0 || !Game::player_is_landed()) && !ship.is_cloaked() && !ship.is_warping()){
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

        double new_distance=Math::get_distance_between_points(box.get_center(),ship.get_box().get_center());

        if(nearest_index==-1 || new_distance<nearest_distance){
            nearest_index=valid_targets[i];
            nearest_distance=new_distance;
        }
    }

    return nearest_index;
}

bool Ship::fire_weapon(const Quadtree<double,uint32_t>& quadtree_ships,RNG& rng,uint32_t own_index){
    bool is_player=own_index==0;

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

                string damage_type=Game_Data::get_shot_type(upgrade->shot_type)->damage_type;

                int32_t damage_mod_for_one_shot=0;

                if(damage_type=="solid"){
                    damage_mod_for_one_shot=get_damage_mod_solid();

                    if (is_player) {
                        Screen_Shake::add_shake(Game_Constants::SHAKE_MAGNITUDE_WEAPON_SOLID, Game_Constants::SHAKE_LENGTH_WEAPON_SOLID);
                        Controller_Manager::make_rumble(Controller_Manager::CONTROLLER_ID_ALL, Game_Constants::SHAKE_MAGNITUDE_WEAPON_SOLID, Game_Constants::SHAKE_LENGTH_WEAPON_SOLID);
                    }
                }
                else if(damage_type=="explosive"){
                    damage_mod_for_one_shot=get_damage_mod_explosive();

                    if (is_player) {
                        Screen_Shake::add_shake(Game_Constants::SHAKE_MAGNITUDE_WEAPON_EXPLOSIVE, Game_Constants::SHAKE_LENGTH_WEAPON_EXPLOSIVE);
                        Controller_Manager::make_rumble(Controller_Manager::CONTROLLER_ID_ALL, Game_Constants::SHAKE_MAGNITUDE_WEAPON_EXPLOSIVE, Game_Constants::SHAKE_LENGTH_WEAPON_EXPLOSIVE);
                    }
                }
                else if(damage_type=="energy"){
                    damage_mod_for_one_shot=get_damage_mod_energy();

                    if (is_player) {
                        Screen_Shake::add_shake(Game_Constants::SHAKE_MAGNITUDE_WEAPON_ENERGY, Game_Constants::SHAKE_LENGTH_WEAPON_ENERGY);
                        Controller_Manager::make_rumble(Controller_Manager::CONTROLLER_ID_ALL, Game_Constants::SHAKE_MAGNITUDE_WEAPON_ENERGY, Game_Constants::SHAKE_LENGTH_WEAPON_ENERGY);
                    }
                }

                int32_t actual_damage_mod=damage_mod_for_one_shot/(int32_t)upgrade->shots;

                if(damage_mod_for_one_shot>0 && actual_damage_mod<1){
                    actual_damage_mod=1;
                }
                else if(damage_mod_for_one_shot<0 && actual_damage_mod>-1){
                    actual_damage_mod=-1;
                }

                Game::create_shot(own_index,upgrade->shot_type,get_faction(),upgrade->name,Coords<double>(box.center_x(),box.center_y()),box.get_angle_to_rect(ship.get_box())+angle_variation,actual_damage_mod);
            }

            if(upgrade->sound.length()>0){
                Sound_Manager::play_sound(upgrade->sound,box.center_x(),box.center_y());
            }

            return true;
        }
    }

    return false;
}

bool Ship::does_active_need_power(string active_name) const{
    //If we would be toggling the cloak off
    if(active_name=="cloak" && is_cloaked()){
        return false;
    }
    //If we would be toggling the warp drive off
    else if(active_name=="warp_drive" && is_warping()){
        return false;
    }
    else{
        return true;
    }
}

bool Ship::can_use_active(bool is_player) const{
    if(has_active() && !is_disabled(is_player)){
        string active_name=get_active_name();
        Upgrade* upgrade=Game_Data::get_upgrade_type(active_name);

        uint32_t cool_point=get_cooldown(upgrade->cooldown)*Engine::UPDATE_RATE/1000;

        if(active_cooldown>=cool_point){
            if(!is_player || !does_active_need_power(active_name) || upgrade->power_use*Engine::UPDATE_RATE<=Game::get_power()){
                return true;
            }
        }
    }

    return false;
}

void Ship::use_active(bool is_player){
    if(can_use_active(is_player)){
        string active_name=get_active_name();
        Upgrade* upgrade=Game_Data::get_upgrade_type(active_name);

        string sound=upgrade->sound;

        if(active_name=="scanner"){
            if(is_player){
                Game::use_power(upgrade->power_use*Engine::UPDATE_RATE);
            }

            active_cooldown=0;

            scanner_startup_sprite.reset_animation();
            scanner_startup_sprite.animating=true;
        }
        else if(active_name=="emp"){
            if(is_player){
                Game::use_power(upgrade->power_use*Engine::UPDATE_RATE);
            }

            active_cooldown=0;

            Game::create_explosion("emp","emp",box.get_center(),0,get_faction(),false,true);

            //We don't want this explosion to affect its user
            damaged_by_explosion(Game::get_explosion_count()-1);
        }
        else if(active_name=="cloak"){
            if(is_player && !is_cloaked()){
                Game::use_power(upgrade->power_use*Engine::UPDATE_RATE);
            }

            toggle_cloak();

            if(!is_cloaked()){
                sound=upgrade->off_sound;
            }
        }
        else if(active_name=="warp_drive"){
            if(is_player && !is_warping()){
                Game::use_power(upgrade->power_use*Engine::UPDATE_RATE);
            }

            toggle_warp();

            if(!is_warping()){
                sound=upgrade->off_sound;
            }
        }

        if(sound.length()>0){
            Sound_Manager::play_sound(sound,box.center_x(),box.center_y());
        }
    }
}

int32_t Ship::get_nearest_valid_target_shot(const Quadtree<double,uint32_t>& quadtree_shots,const Collision_Rect<double>& box_targeting){
    vector<uint32_t> nearby_shots;
    quadtree_shots.get_objects(nearby_shots,box_targeting);

    vector<uint32_t> valid_targets;

    unordered_set<uint32_t> collisions;

    //Find all valid targets
    for(size_t i=0;i<nearby_shots.size();i++){
        if(!collisions.count(nearby_shots[i])){
            collisions.emplace(nearby_shots[i]);

            const Shot& shot=Game::get_shot(nearby_shots[i]);

            if(shot.is_alive() && shot.get_shot_type()->damage_type=="explosive"){
                Collision_Rect<double> box_shot=shot.get_box();

                if(faction_is_valid(shot.get_faction(),true) && Collision::check_rect(box_targeting,box_shot)){
                    valid_targets.push_back(nearby_shots[i]);
                }
            }
        }
    }

    //Find nearest valid target
    int32_t nearest_index=-1;
    double nearest_distance=0.0;

    for(size_t i=0;i<valid_targets.size();i++){
        const Shot& shot=Game::get_shot(valid_targets[i]);

        double new_distance=Math::get_distance_between_points(box.get_center(),shot.get_box().get_center());

        if(nearest_index==-1 || new_distance<nearest_distance){
            nearest_index=valid_targets[i];
            nearest_distance=new_distance;
        }
    }

    return nearest_index;
}

bool Ship::fire_point_defense(const Quadtree<double,uint32_t>& quadtree_shots){
    if(has_point_defense()){
        Collision_Rect<double> box_targeting=box;

        box_targeting.x-=Game_Constants::POINT_DEFENSE_RANGE;
        box_targeting.y-=Game_Constants::POINT_DEFENSE_RANGE;
        box_targeting.w+=Game_Constants::POINT_DEFENSE_RANGE*2.0;
        box_targeting.h+=Game_Constants::POINT_DEFENSE_RANGE*2.0;

        int32_t nearest_index=get_nearest_valid_target_shot(quadtree_shots,box_targeting);

        //If a nearest valid target was found
        if(nearest_index>=0){
            const Shot& shot=Game::get_shot((uint32_t)nearest_index);

            Game::create_explosion("explosion_missile","explosion_missile",Coords<double>(shot.get_box().center_x(),shot.get_box().center_y()),shot.get_damage(),shot.get_faction());

            Game::create_effect("",true,1.0,box.get_center(),"point_defense",Vector(),0.0,Vector(),1,true,shot.get_box().get_center(),"point_defense");

            Game::kill_shot((uint32_t)nearest_index);

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

    reset_ai_ignore_angle_mods();
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

bool Ship::ai_proximity_target_is_player() const{
    return ai_has_proximity_target && ai_proximity_target==0;
}

void Ship::ai(const Quadtree<double,uint32_t>& quadtree_ships,const Quadtree<double,uint32_t>& quadtree_planets,const Quadtree<double,uint32_t>& quadtree_debris,
              uint32_t frame,uint32_t own_index,RNG& rng){
    if(is_alive() && !is_landing() && !is_disabled(false) && is_in_processing_range()){
        if(ai_proximity_check_allowed(frame,own_index)){
            ai_check_for_proximity_target(quadtree_ships,own_index);
        }

        thrusting=false;
        set_braking(false);

        if(get_faction()=="civilian"){
            vector<uint32_t> nearby_planets;
            quadtree_planets.get_objects(nearby_planets,box);

            unordered_set<uint32_t> collisions;

            for(size_t i=0;i<nearby_planets.size();i++){
                if(!collisions.count(nearby_planets[i])){
                    collisions.emplace(nearby_planets[i]);

                    const Planet& planet=Game::get_planet(nearby_planets[i]);

                    vector<Coords<double>> vertices;
                    box.get_vertices(vertices,angle);

                    if(Game::is_object_over_planet(vertices,planet)){
                        commence_landing(nearby_planets[i]);

                        return;
                    }
                }
            }
        }

        if(ai_has_proximity_target){
            angle=Math::get_angle_to_point(box.get_center(),Game::get_ship(ai_proximity_target).box.get_center());

            if(ai_proximity_target_flee){
                angle+=180.0;
            }

            if(get_faction()=="police" && ai_proximity_target_is_player() && Game::notoriety_tier_1() && get_distance_to_player()<=Game_Constants::TRACTOR_RANGE){
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

                    reset_ai_ignore_angle_mods();
                }
            }
            else if(get_faction()=="pirate" || get_faction()=="bounty_hunter"){
                if(Collision::check_rect(box,Collision_Rect<double>(ai_target.x,ai_target.y,1.0,1.0))){
                    ai_select_target(own_index,rng);
                }
            }

            ai_determine_angle(quadtree_debris);

            thrusting=true;
        }

        //If not a police ship, use active
        //If a police ship, only use active if not currently following or tractoring the player
        if(get_faction()!="police" || (!ai_proximity_target_is_player() && (!Game::is_player_tractored() || Game::get_tractoring_ship_index()!=own_index))){
            use_active(false);
        }
    }
}

void Ship::ai_determine_angle(const Quadtree<double,uint32_t>& quadtree_debris){
    Vector angle_determinant(Game_Constants::AI_WEIGHT_TARGET,Math::get_angle_to_point(box.get_center(),ai_target));

    if(!ai_ignore_angle_mods && Math::get_distance_between_points(box.get_center(),ai_target)>Game_Constants::AI_ANGLE_MOD_IGNORE_DISTANCE){
        Collision_Rect<double> box_check(box.x-Game_Constants::AI_ANGLE_MOD_DISTANCE,box.y-Game_Constants::AI_ANGLE_MOD_DISTANCE,
                                         box.w+Game_Constants::AI_ANGLE_MOD_DISTANCE*2.0,box.h+Game_Constants::AI_ANGLE_MOD_DISTANCE*2.0);

        vector<uint32_t> nearby_debris;
        quadtree_debris.get_objects(nearby_debris,box_check);

        unordered_set<uint32_t> collisions;

        for(size_t i=0;i<nearby_debris.size();i++){
            if(!collisions.count(nearby_debris[i])){
                collisions.emplace(nearby_debris[i]);

                const Debris& debris=Game::get_debris(nearby_debris[i]);
                Collision_Rect<double> box_debris=debris.get_box();

                double object_size=(box_debris.w+box_debris.h)/2.0;

                double object_distance=Math::get_distance_between_points(box.get_center(),box_debris.get_center());
                if(object_distance<=0.0){
                    object_distance=0.0000001;
                }

                angle_determinant+=Vector((Game_Constants::AI_WEIGHT_AVOID_DEBRIS*object_size)/(object_distance*object_distance),Math::get_angle_to_point(box_debris.get_center(),box.get_center()));
            }
        }
    }

    angle=angle_determinant.direction;
}

void Ship::thrust(uint32_t frame){
    if(thrusting){
        double thrust_magnitude=get_thrust_accel();

        if(Math::get_distance_between_angles(velocity.direction,angle)>=Game_Constants::SHIP_MANEUVER_ANGLE){
            thrust_magnitude=get_thrust_decel();
        }

        Vector thrust_force(thrust_magnitude,angle);

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

        if(brake_force.magnitude/get_mass()>velocity.magnitude){
            brake_force.magnitude=velocity.magnitude*get_mass();
        }

        force+=brake_force;

        if(frame%30==0){
            Sound_Manager::play_sound("brake",box.center_x(),box.center_y());
        }
    }
}

void Ship::accelerate(bool is_player,uint32_t frame){
    if(is_alive() && !is_landing() && (!is_player || !Game::player_is_landed()) && is_in_processing_range()){
        thrust(frame);
        brake(frame);

        if(is_player && Game::is_player_tractored()){
            apply_tractor(Game::get_tractor_angle());
        }

        Vector acceleration=force/get_mass();

        if(is_player){
            Game::set_player_acceleration(acceleration);
        }

        velocity+=acceleration;

        if(velocity.magnitude>get_max_speed()){
            velocity.magnitude=get_max_speed();
        }

        force*=0.0;
    }
}

void Ship::rotation (bool is_player) {
    if (is_alive() && !is_landing() && (!is_player || !Game::player_is_landed()) && is_in_processing_range() && get_angular_velocity().magnitude > 0.0) {
        if (get_angular_velocity().direction >= 0.0 && get_angular_velocity().direction < 180.0) {
            render_angle += get_angular_velocity().magnitude;
        } else {
            render_angle -= get_angular_velocity().magnitude;
        }
    }
}

void Ship::movement(uint32_t own_index,const Quadtree<double,uint32_t>& quadtree_debris,const Quadtree<double,uint32_t>& quadtree_shots,
                    const Quadtree<double,uint32_t>& quadtree_explosions,const Quadtree<double,uint32_t>& quadtree_items,RNG& rng){
    bool is_player=own_index==0;

    if(is_alive() && !is_landing() && (!is_player || !Game::player_is_landed()) && is_in_processing_range()){
        double last_x=box.x;
        double last_y=box.y;

        Vector_Components vc=velocity.get_components();

        box.x+=vc.a/(double)Engine::UPDATE_RATE;
        box.y+=vc.b/(double)Engine::UPDATE_RATE;

        if(!ai_ignore_angle_mods){
            if(Math::abs(box.x-last_x)+Math::abs(box.y-last_y)<Game_Constants::AI_TIME_WITHOUT_MOVING_DISTANCE_THRESHOLD){
                time_without_moving++;
            }
            else{
                time_without_moving=0;
            }

            if(time_without_moving>=Game_Constants::AI_TIME_WITHOUT_MOVING_ALLOWED*Engine::UPDATE_RATE/1000){
                ai_ignore_angle_mods=true;
            }
        }

        if(!is_warping()){
            vector<uint32_t> nearby_debris;
            quadtree_debris.get_objects(nearby_debris,box);

            unordered_set<uint32_t> collisions;

            for(size_t i=0;i<nearby_debris.size();i++){
                if(is_alive()){
                    if(!collisions.count(nearby_debris[i])){
                        collisions.emplace(nearby_debris[i]);

                        const Debris& debris=Game::get_debris(nearby_debris[i]);

                        vector<Coords<double>> vertices_collision;
                        debris.get_collision_box().get_vertices(vertices_collision, debris.get_angle());
                        if(rng.random_range(0,99)<Game_Constants::COLLISION_CHANCE_DEBRIS && Collision::check_vertices_rect(get_collision_vertices(),vertices_collision)){
                            string damage_faction="world";

                            //If we are either fleeing from or chasing a ship, it is blamed for this damage
                            if(!is_player && ai_has_proximity_target){
                                damage_faction=Game::get_ship(ai_proximity_target).get_faction();
                            }

                            take_damage(is_player,debris.get_debris_type()->damage,debris.get_debris_type()->damage_type,
                                        Coords<double>(box.x+rng.random_range(0,(uint32_t)box.w),box.y+rng.random_range(0,(uint32_t)box.h)),damage_faction,rng);
                        }
                    }
                }
                else{
                    return;
                }
            }

            vector<uint32_t> nearby_shots;
            quadtree_shots.get_objects(nearby_shots,box);

            collisions.clear();

            for(size_t i=0;i<nearby_shots.size();i++){
                if(is_alive()){
                    if(!collisions.count(nearby_shots[i])){
                        collisions.emplace(nearby_shots[i]);

                        const Shot& shot=Game::get_shot(nearby_shots[i]);

                        if(shot.is_alive() && (!shot.has_owner() || own_index!=shot.get_owner_index())){
                            Collision_Rect<double> box_shot = shot.get_collision_box();

                            vector<Coords<double>> vertices_collision;
                            box_shot.get_vertices(vertices_collision, shot.get_angle());
                            if (Collision::check_vertices_rect(get_collision_vertices(),vertices_collision)) {
                                if(shot.get_shot_type()->damage_type=="explosive"){
                                    Game::create_explosion("explosion_missile","explosion_missile",Coords<double>(box_shot.center_x(),box_shot.center_y()),shot.get_damage(),shot.get_faction());
                                }
                                else{
                                    take_damage(is_player,shot.get_damage(),shot.get_shot_type()->damage_type,
                                                Coords<double>(box.x+rng.random_range(0,(uint32_t)box.w),box.y+rng.random_range(0,(uint32_t)box.h)),shot.get_faction(),rng);
                                }

                                Game::kill_shot(nearby_shots[i]);
                            }
                        }
                    }
                }
                else{
                    return;
                }
            }

            vector<uint32_t> nearby_explosions;
            quadtree_explosions.get_objects(nearby_explosions,box);

            collisions.clear();

            for(size_t i=0;i<nearby_explosions.size();i++){
                if(is_alive()){
                    if(!collisions.count(nearby_explosions[i])){
                        collisions.emplace(nearby_explosions[i]);

                        const Explosion& explosion=Game::get_explosion(nearby_explosions[i]);

                        if(explosion.is_alive() && !was_damaged_by_explosion(nearby_explosions[i])){
                            Coords<double> collision_center = Collision::get_vertices_center_rect(get_collision_vertices());

                            if (Collision::check_circ(explosion.get_circle(),Collision_Circ<double>(collision_center.x, collision_center.y, (get_collision_box().w + get_collision_box().h) / 2.0))) {
                                damaged_by_explosion(nearby_explosions[i]);

                                if(explosion.is_scan()){
                                    if(is_player){
                                        //Disable cloak
                                        if(is_cloaked()){
                                            toggle_cloak();
                                        }

                                        if(Game::player_has_contract()){
                                            if (Game::notoriety_tier_1() || Game::notoriety_tier_2()) {
                                                Game::increase_notoriety(Game_Constants::NOTORIETY_INCREASE_SCAN);
                                            } else {
                                                Game::increase_notoriety(Game_Constants::NOTORIETY_INCREASE_SCAN_INITIAL);
                                            }
                                        }
                                    }
                                }
                                else if(explosion.is_emp()){
                                    disable();
                                }
                                else{
                                    take_damage(is_player,explosion.get_damage(),"explosive",
                                                Coords<double>(box.x+rng.random_range(0,(uint32_t)box.w),box.y+rng.random_range(0,(uint32_t)box.h)),
                                                explosion.get_faction(),rng);
                                }
                            }
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

                    if(Collision::check_vertices_rect(get_collision_vertices(),ship.get_collision_vertices())){
                        Game::arrest_player();
                    }
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

        cast_shadow = false;

        vector<Coords<double>> vertices;
        Collision_Rect<double> shadow_box = get_box();
        shadow_box.w += Game_Constants::SHIP_SHADOW_OFFSET;
        shadow_box.h += Game_Constants::SHIP_SHADOW_OFFSET;
        shadow_box.get_vertices(vertices, get_angle());

        for (uint32_t i = 0; i < Game::get_planet_count(); i++) {
            if (Game::should_object_cast_shadow_on_planet(vertices, Game::get_planet(i))) {
                cast_shadow = true;

                break;
            }
        }
    }
}

void Ship::animate_scanner_startup(){
    if(scanner_startup_sprite.animating){
        scanner_startup_sprite.animate();

        //If we just finished the scanner startup animation
        if(!scanner_startup_sprite.animating){
            Game::create_explosion("scanner_scan","scanner",box.get_center(),0,get_faction(),true);

            //We don't want this explosion to affect its user
            damaged_by_explosion(Game::get_explosion_count()-1);
        }
    }
}

void Ship::animate(bool tractoring){
    if(is_alive()){
        if(thrusting){
            sprite.animate();

            if (sprite.frame == 0) {
                sprite.frame++;
            }
        }
        else{
            sprite.reset_animation();
        }

        animate_scanner_startup();

        if(tractoring ||
           (get_faction()=="police" && ai_proximity_target_is_player() && !is_disabled(false) && (Game::notoriety_tier_1() || Game::notoriety_tier_2()))){
            if(++sound_cooldown_chasing_player>=Game_Constants::CHASING_PLAYER_SOUND_RATE*Engine::UPDATE_RATE/1000){
                sound_cooldown_chasing_player=0;

                Sound_Manager::play_sound("siren",box.center_x(),box.center_y());
            }
        }
    }
}

void Ship::render(bool tractoring,bool is_player){
    if(is_alive()){
        double angle_to_use = angle;

        if (get_angular_velocity().magnitude > 0.0) {
            angle_to_use = render_angle;
        }

        bool in_camera=Collision::check_rect_rotated(box*Game_Manager::camera_zoom,Game_Manager::camera,angle_to_use,0.0);

        if(in_camera){
            if(is_player || !is_cloaked()){
                double opacity=1.0;
                if(is_cloaked()){
                    opacity=Game_Constants::CLOAK_OPACITY;
                }

                double scale=1.0;
                if(is_landing()){
                    scale=landing_scale;
                }

                if (cast_shadow) {
                    sprite.render((box.x + Game_Constants::SHIP_SHADOW_OFFSET)*Game_Manager::camera_zoom-Game_Manager::camera.x,(box.y + Game_Constants::SHIP_SHADOW_OFFSET)*Game_Manager::camera_zoom-Game_Manager::camera.y,0.25,scale*0.65,scale*0.65,angle_to_use,"ui_black");
                }

                sprite.render(box.x*Game_Manager::camera_zoom-Game_Manager::camera.x,box.y*Game_Manager::camera_zoom-Game_Manager::camera.y,opacity,scale,scale,angle_to_use);
            }

            if (Game_Options::show_collision_outlines) {
                Collision_Rect<double> dodge_box(box.x - Game_Constants::DODGE_RANGE, box.y - Game_Constants::DODGE_RANGE, box.w + Game_Constants::DODGE_RANGE*2.0, box.h+ Game_Constants::DODGE_RANGE*2.0);
                Render::render_rectangle(dodge_box.x * Game_Manager::camera_zoom - Game_Manager::camera.x, dodge_box.y * Game_Manager::camera_zoom - Game_Manager::camera.y, dodge_box.w, dodge_box.h, 0.25, "text_input_blue");

                vector<Coords<double>> vertices_collision = get_collision_vertices();

                for (size_t i = 0; i < vertices_collision.size(); i++) {
                    uint32_t start_vertex=(uint32_t)i;
                    uint32_t end_vertex=(i<vertices_collision.size()-1) ? start_vertex+1 : 0;

                    Render::render_line(vertices_collision[start_vertex].x*Game_Manager::camera_zoom-Game_Manager::camera.x,vertices_collision[start_vertex].y*Game_Manager::camera_zoom-Game_Manager::camera.y,
                                        vertices_collision[end_vertex].x*Game_Manager::camera_zoom-Game_Manager::camera.x,vertices_collision[end_vertex].y*Game_Manager::camera_zoom-Game_Manager::camera.y,1.0,"red");
                }
            }
        }

        if(tractoring){
            const Sprite& tractor_sprite=Game::get_tractor_sprite();

            double x=box.center_x()-tractor_sprite.get_width()/2.0;
            double y=box.center_y()-tractor_sprite.get_height()/2.0;

            Coords<double> box_side(box.x+box.w,box.center_y());
            box_side=Math::rotate_point(box_side,box.get_center(),angle);

            Vector move_sprite(Math::get_distance_between_points(box.get_center(),box_side),angle);
            Vector_Components vc=move_sprite.get_components();
            x+=vc.a;
            y+=vc.b;

            tractor_sprite.render(x*Game_Manager::camera_zoom-Game_Manager::camera.x,y*Game_Manager::camera_zoom-Game_Manager::camera.y,1.0,1.0,1.0,angle);
        }

        if(in_camera && get_faction()=="police" && ai_proximity_target_is_player() && !is_disabled(false) && (Game::notoriety_tier_1() || Game::notoriety_tier_2())){
            const Sprite& police_lights_sprite=Game::get_police_lights_sprite();

            double x=box.center_x()-police_lights_sprite.get_width()/2.0;
            double y=box.center_y()-police_lights_sprite.get_height()/2.0;

            double light_angle=Game::get_police_lights_angle()+angle_to_use;

            police_lights_sprite.render(x*Game_Manager::camera_zoom-Game_Manager::camera.x,y*Game_Manager::camera_zoom-Game_Manager::camera.y,1.0,1.0,1.0,light_angle);
        }

        if(in_camera && scanner_startup_sprite.animating){
            double x=box.center_x()-scanner_startup_sprite.get_width()/2.0;
            double y=box.center_y()-scanner_startup_sprite.get_height()/2.0;

            scanner_startup_sprite.render(x*Game_Manager::camera_zoom-Game_Manager::camera.x,y*Game_Manager::camera_zoom-Game_Manager::camera.y,1.0,1.0,1.0,angle_to_use);
        }
    }
}
