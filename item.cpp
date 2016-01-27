/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "item.h"
#include "game_data.h"
#include "game.h"

#include <game_manager.h>
#include <sound_manager.h>
#include <engine.h>
///QQQ includes
///#include <render.h>
///

using namespace std;

Item::Item(string new_type,const Coords<double>& position,const Vector& new_velocity,double new_angle,const Vector& new_angular_velocity){
    type=new_type;

    box.x=position.x;
    box.y=position.y;

    sprite.set_name(get_item_type()->sprite);

    box.w=sprite.get_width();
    box.h=sprite.get_height();

    velocity=new_velocity;

    angle=new_angle;
    angular_velocity=new_angular_velocity;

    alive=true;
}

Item_Type* Item::get_item_type() const{
    return Game_Data::get_item_type(type);
}

Collision_Rect<double> Item::get_box() const{
    return box;
}

bool Item::is_alive() const{
    return alive;
}

void Item::die(){
    if(is_alive()){
        alive=false;
    }
}

void Item::play_collection_sound() const{
    string sound=get_item_type()->collect_sound;

    if(sound.length()>0){
        Sound_Manager::play_sound(sound,box.center_x(),box.center_y());
    }
}

void Item::brake(){
    angular_velocity.magnitude-=0.001;

    if(angular_velocity.magnitude<0.0){
        angular_velocity.magnitude=0.0;
    }

    Vector brake_force(get_item_type()->thrust_decel,velocity.direction+180.0);

    Math::clamp_angle(brake_force.direction);

    if(brake_force.magnitude/get_item_type()->mass>velocity.magnitude){
        brake_force.magnitude=velocity.magnitude*get_item_type()->mass;
    }

    force+=brake_force;
}

void Item::accelerate(){
    if(is_alive()){
        brake();

        Vector acceleration=force/get_item_type()->mass;

        velocity+=acceleration;

        force*=0.0;
    }
}

void Item::rotation(){
    if(is_alive() && angular_velocity.magnitude>0.0){
        if(angular_velocity.direction>=0 && angular_velocity.direction<180){
            angle+=angular_velocity.magnitude;
        }
        else{
            angle-=angular_velocity.magnitude;
        }
    }
}

void Item::movement(){
    if(is_alive()){
        Vector_Components vc=velocity.get_components();

        box.x+=vc.a/(double)Engine::UPDATE_RATE;
        box.y+=vc.b/(double)Engine::UPDATE_RATE;

        if(box.x+box.w<0.0){
            die();
        }
        if(box.y+box.h<0.0){
            die();
        }
        if(box.x>=Game::world_width){
            die();
        }
        if(box.y>=Game::world_height){
            die();
        }
    }
}

void Item::animate(){
    if(is_alive()){
        sprite.animate();
    }
}

void Item::render(){
    if(is_alive()){
        if(Collision::check_rect(box*Game_Manager::camera_zoom,Game_Manager::camera)){
            sprite.render(box.x*Game_Manager::camera_zoom-Game_Manager::camera.x,box.y*Game_Manager::camera_zoom-Game_Manager::camera.y,1.0,1.0,1.0,angle);

            ///QQQ render collision box
            ///Render::render_rectangle(box.x*Game_Manager::camera_zoom-Game_Manager::camera.x,box.y*Game_Manager::camera_zoom-Game_Manager::camera.y,box.w,box.h,0.25,"red");
            ///
        }
    }
}
