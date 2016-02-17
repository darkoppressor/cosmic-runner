/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "effect.h"
#include "game_constants.h"

#include <game_manager.h>
#include <sound_manager.h>
#include <engine.h>
#include <render.h>

using namespace std;

Effect::Effect(std::string new_sprite,bool new_fade,double new_scale,const Coords<double>& position,string sound,const Vector& new_velocity,double new_angle,
               const Vector& new_angular_velocity,uint32_t seconds,bool new_line,const Coords<double>& end_point,string new_color){
    done=false;
    opacity=1.0;
    fade=new_fade;

    line=new_line;

    if(new_sprite.length()>0){
        sprite.set_name(new_sprite);
    }

    scale=new_scale;

    if(!line){
        box.w=sprite.get_width()*scale;
        box.h=sprite.get_height()*scale;

        box.x=position.x-box.w/2.0;
        box.y=position.y-box.h/2.0;
    }
    else{
        box.x=position.x;
        box.y=position.y;

        box.w=end_point.x;
        box.h=end_point.y;
    }

    velocity=new_velocity;

    angle=new_angle;

    angular_velocity=new_angular_velocity;

    color=new_color;

    if(seconds>0){
        timed=true;

        counter=seconds*Engine::UPDATE_RATE;
    }
    else{
        timed=false;

        counter=0;
    }

    if(sound.length()>0){
        Sound_Manager::play_sound(sound,box.center_x(),box.center_y());
    }
}

bool Effect::is_fading() const{
    if(timed){
        return counter==0;
    }
    else{
        return !sprite.animating;
    }
}

bool Effect::is_done() const{
    return done;
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
        if(!line){
            sprite.animate();
        }

        if(is_fading()){
            if(fade){
                opacity-=Game_Constants::EFFECT_FADE_RATE;
            }
            else{
                opacity=0.0;
            }

            if(opacity<=0.0){
                opacity=0.0;

                done=true;
            }
        }
    }
}

void Effect::render(){
    if(!is_done()){
        if(!line){
            if(Collision::check_rect(box*Game_Manager::camera_zoom,Game_Manager::camera)){
                sprite.render(box.x*Game_Manager::camera_zoom-Game_Manager::camera.x,box.y*Game_Manager::camera_zoom-Game_Manager::camera.y,opacity,scale,scale,angle,color);
            }
        }
        else if(Collision::check_rect(Collision_Rect<double>(box.x,box.y,1.0,1.0)*Game_Manager::camera_zoom,Game_Manager::camera) ||
                Collision::check_rect(Collision_Rect<double>(box.w,box.h,1.0,1.0)*Game_Manager::camera_zoom,Game_Manager::camera)){
            Render::render_line(box.x*Game_Manager::camera_zoom-Game_Manager::camera.x,box.y*Game_Manager::camera_zoom-Game_Manager::camera.y,
                                box.w*Game_Manager::camera_zoom-Game_Manager::camera.x,box.h*Game_Manager::camera_zoom-Game_Manager::camera.y,opacity,color);
        }
    }
}
