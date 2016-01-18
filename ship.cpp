/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "ship.h"
#include "game_data.h"
#include "game.h"
#include "game_constants.h"

#include <engine.h>
#include <game_manager.h>
///QQQ includes
///#include <render.h>

using namespace std;

Ship::Ship(string new_type,const Coords<double>& position,double new_angle){
    type=new_type;

    box.x=position.x;
    box.y=position.y;

    angle=new_angle;

    hull=get_ship_type()->hull_max;
    shields=get_ship_type()->shields_max;

    thrusting=false;
    braking=false;

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

Collision_Rect<double> Ship::get_collision_box() const{
    return Collision_Rect<double>(box.x+box.w/2.0-box.w*get_ship_type()->collision_percentage/2.0,box.y+box.h/2.0-box.h*get_ship_type()->collision_percentage/2.0,
                                  box.w*get_ship_type()->collision_percentage,box.h*get_ship_type()->collision_percentage);
}

bool Ship::is_alive() const{
    return hull>0;
}

void Ship::take_damage(int32_t damage,string damage_type,const Coords<double>& location){
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
                Game::create_effect("explosion_shields",((double)effective_damage/(double)get_ship_type()->shields_max)/2.0,location,"explosion_shields");

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
            Game::create_effect("explosion_hull",((double)effective_damage/(double)get_ship_type()->hull_max)/2.0,location,"explosion_hull");

            hull-=effective_damage;
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

void Ship::thrust(){
    if(thrusting){
        Vector thrust_force(get_ship_type()->thrust_accel,angle);

        force+=thrust_force;
    }
}

void Ship::brake(){
    if(braking){
        Vector brake_force(get_ship_type()->thrust_decel,velocity.direction+180.0);

        Math::clamp_angle(brake_force.direction);

        if(brake_force.magnitude/get_ship_type()->mass>velocity.magnitude){
            brake_force.magnitude=velocity.magnitude*get_ship_type()->mass;
        }

        force+=brake_force;
    }
}

void Ship::accelerate(){
    if(is_alive()){
        thrust();
        brake();

        Vector acceleration=force/get_ship_type()->mass;

        velocity+=acceleration;

        if(velocity.magnitude>get_ship_type()->max_speed){
            velocity.magnitude=get_ship_type()->max_speed;
        }
        else if(velocity.magnitude<-get_ship_type()->max_speed){
            velocity.magnitude=-get_ship_type()->max_speed;
        }

        force*=0.0;
    }
}

void Ship::movement(const Quadtree<double,uint32_t>& quadtree_debris,RNG& rng){
    if(is_alive()){
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

                take_damage(debris.get_debris_type()->damage,debris.get_debris_type()->damage_type,Coords<double>(box_area.center_x(),box_area.center_y()));
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
            sprite.render(box.x*Game_Manager::camera_zoom-Game_Manager::camera.x,box.y*Game_Manager::camera_zoom-Game_Manager::camera.y,1.0,1.0,1.0,angle);

            ///QQQ render collision boxes
            /**Collision_Rect<double> col_box=get_collision_box();
            Render::render_rectangle(box.x*Game_Manager::camera_zoom-Game_Manager::camera.x,box.y*Game_Manager::camera_zoom-Game_Manager::camera.y,box.w,box.h,0.5,"ui_white");
            Render::render_rectangle(col_box.x*Game_Manager::camera_zoom-Game_Manager::camera.x,col_box.y*Game_Manager::camera_zoom-Game_Manager::camera.y,col_box.w,col_box.h,0.5,"red");*/
            ///
        }
    }
}
