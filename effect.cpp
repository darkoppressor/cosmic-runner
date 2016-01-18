/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "effect.h"

#include <game_manager.h>
#include <sound_manager.h>

using namespace std;

Effect::Effect(std::string new_sprite,double new_scale,const Coords<double>& position,string new_sound){
    sprite.set_name(new_sprite);

    scale=new_scale;

    if(scale<0.5){
        scale=0.5;
    }

    box.w=sprite.get_width()*scale;
    box.h=sprite.get_height()*scale;

    box.x=position.x-box.w/2.0;
    box.y=position.y-box.h/2.0;

    sound=new_sound;
}

bool Effect::is_done() const{
    return !sprite.animating;
}

void Effect::play_sound(){
    if(sound.length()>0){
        Sound_Manager::play_sound(sound,box.center_x(),box.center_y());
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
            sprite.render(box.x*Game_Manager::camera_zoom-Game_Manager::camera.x,box.y*Game_Manager::camera_zoom-Game_Manager::camera.y,1.0,scale,scale);
        }
    }
}
