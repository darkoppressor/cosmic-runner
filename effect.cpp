/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "effect.h"

#include <game_manager.h>
#include <sound_manager.h>
#include <engine.h>

using namespace std;

Effect::Effect(std::string new_sprite,double new_scale,const Coords<double>& position,string new_sound,const Vector& new_velocity,double new_angle,
               const Vector& new_angular_velocity,uint32_t seconds){
    sprite.set_name(new_sprite);

    scale=new_scale;

    box.w=sprite.get_width()*scale;
    box.h=sprite.get_height()*scale;

    box.x=position.x-box.w/2.0;
    box.y=position.y-box.h/2.0;

    sound=new_sound;

    velocity=new_velocity;

    angle=new_angle;

    angular_velocity=new_angular_velocity;

    if(seconds>0){
        timed=true;

        counter=seconds*Engine::UPDATE_RATE;
    }
    else{
        timed=false;

        counter=0;
    }
}

bool Effect::is_done() const{
    if(timed){
        return counter==0;
    }
    else{
        return !sprite.animating;
    }
}

void Effect::play_sound(){
    if(sound.length()>0){
        Sound_Manager::play_sound(sound,box.center_x(),box.center_y());
    }
}

void Effect::countdown(){
    if(timed){
        if(counter>0){
            counter--;
        }
    }
}

void Effect::movement(){
    if(!is_done()){
        Vector_Components vc=velocity.get_components();

        box.x+=vc.a/(double)Engine::UPDATE_RATE;
        box.y+=vc.b/(double)Engine::UPDATE_RATE;

        if(angular_velocity.direction>=0 && angular_velocity.direction<180){
            angle+=angular_velocity.magnitude;
        }
        else{
            angle-=angular_velocity.magnitude;
        }
    }
}

void Effect::animate(){
    if(!is_done()){
        sprite.animate();
    }
}

void Effect::render(){
    if(!is_done()){
        if(Collision::check_rect(box*Game_Manager::camera_zoom,Game_Manager::camera)){
            sprite.render(box.x*Game_Manager::camera_zoom-Game_Manager::camera.x,box.y*Game_Manager::camera_zoom-Game_Manager::camera.y,1.0,scale,scale,angle);
        }
    }
}
