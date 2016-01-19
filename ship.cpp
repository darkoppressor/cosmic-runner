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

    sprite.set_name(get_ship_type()->sprite);

    box.w=sprite.get_width();
    box.h=sprite.get_height();
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
                Game::create_effect("explosion_shields",((double)effective_damage/(double)get_shields_max())/2.0,location,"explosion_shields",
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
            Game::create_effect("explosion_hull",((double)effective_damage/(double)get_hull_max())/2.0,location,"explosion_hull",
                                Vector(0.0,0.0),0.0,Vector(0.0,0.0),0);

            hull-=effective_damage;

            if(!is_alive()){
                ///QQQ create explosion

                if(is_player){
                    Game::game_over();
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

void Ship::regenerate_shields(){
    if(shields<get_shields_max() && ++shield_recharge>=get_shield_recharge_rate()*Engine::UPDATE_RATE/1000){
        shield_recharge=0;

        shields++;
    }
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

void Ship::movement(bool is_player,const Quadtree<double,uint32_t>& quadtree_debris,const Quadtree<double,uint32_t>& quadtree_shots,RNG& rng){
    if(is_alive() && !is_landing() && (!is_player || !Game::player_is_landed())){
        Vector_Components vc=velocity.get_components();

        box.x+=vc.a/(double)Engine::UPDATE_RATE;
        box.y+=vc.b/(double)Engine::UPDATE_RATE;

        Collision_Rect<double> box_collision=get_collision_box();

        vector<uint32_t> nearby_debris;
        quadtree_debris.get_objects(nearby_debris,box_collision);

        for(size_t i=0;i<nearby_debris.size();i++){
            const Debris& debris=Game::get_debris(nearby_debris[i]);
            Collision_Rect<double> box_debris=debris.get_collision_box();

            if(rng.random_range(0,99)<Game_Constants::COLLISION_CHANCE_DEBRIS && Collision::check_rect(box_collision,box_debris)){
                Collision_Rect<double> box_area=Collision::get_collision_area_rect(box_collision,box_debris);

                take_damage(is_player,debris.get_debris_type()->damage,debris.get_debris_type()->damage_type,Coords<double>(box_area.center_x(),box_area.center_y()));
            }
        }

        vector<uint32_t> nearby_shots;
        quadtree_shots.get_objects(nearby_shots,box_collision);

        for(size_t i=0;i<nearby_shots.size();i++){
            const Shot& shot=Game::get_shot(nearby_shots[i]);
            Collision_Rect<double> box_shot=shot.get_collision_box();

            if(Collision::check_rect(box_collision,box_shot)){
                Collision_Rect<double> box_area=Collision::get_collision_area_rect(box_collision,box_shot);

                take_damage(is_player,shot.get_shot_type()->damage,shot.get_shot_type()->damage_type,Coords<double>(box_area.center_x(),box_area.center_y()));

                Game::kill_shot(nearby_shots[i]);
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
