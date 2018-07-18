/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "effect.h"
#include "game_constants.h"
#include "game.h"

#include <game_manager.h>
#include <sound_manager.h>
#include <engine.h>
#include <render.h>
#include <object_manager.h>

using namespace std;

Effect::Effect(std::string new_sprite,bool new_fade,double new_scale,const Coords<double>& position,string sound,const Vector& new_velocity,double new_angle,
               const Vector& new_angular_velocity,uint32_t seconds,bool new_line,const Coords<double>& end_point,string new_color,string new_text,bool new_cloud){
    done=false;
    opacity=1.0;
    fade=new_fade;

    line=new_line;

    text=new_text;

    if(new_sprite.length()>0){
        sprite.set_name(new_sprite);
    }

    scale=new_scale;

    if(!line){
        if (text.length() > 0) {
            Bitmap_Font* font=Object_Manager::get_font("standard");

            box.w=font->get_letter_width()*4.0*scale;
            box.h=font->get_letter_height()*scale;
        } else {
            box.w=sprite.get_width()*scale;
            box.h=sprite.get_height()*scale;
        }

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

    cloud=new_cloud;
    cloud_fading = false;

    cast_shadow = false;
}

bool Effect::is_fading() const{
    if (timed) {
        return counter == 0;
    } else if (cloud) {
        return cloud_fading;
    } else {
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
    if (!is_done() && (!cloud || !cloud_fading)) {
        Vector_Components vc=velocity.get_components();

        box.x+=vc.a/(double)Engine::UPDATE_RATE;
        box.y+=vc.b/(double)Engine::UPDATE_RATE;

        if(angular_velocity.direction>=0 && angular_velocity.direction<180){
            angle+=angular_velocity.magnitude;
        }
        else{
            angle-=angular_velocity.magnitude;
        }

        if (cloud) {
            cloud_fading = true;
            cast_shadow = false;

            vector<Coords<double>> vertices_cloud;
            box.get_vertices(vertices_cloud,0.0);

            vector<Coords<double>> vertices_shadow;
            Collision_Rect<double> shadow_box = box;
            shadow_box.w += Game_Constants::SHADOW_OFFSET;
            shadow_box.h += Game_Constants::SHADOW_OFFSET;
            shadow_box.get_vertices(vertices_shadow, angle);

            for (uint32_t i = 0; i < Game::get_planet_count(); i++) {
                if (Game::should_object_cast_shadow_on_planet(vertices_cloud, Game::get_planet(i))) {
                    cloud_fading = false;

                    if (cast_shadow) {
                        break;
                    }
                }

                if (Game::should_object_cast_shadow_on_planet(vertices_shadow, Game::get_planet(i))) {
                    cast_shadow = true;

                    if (!cloud_fading) {
                        break;
                    }
                }
            }
        }
    }
}

void Effect::animate(){
    if(!is_done()){
        if(!line && text.length() == 0){
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

void Effect::render(bool clouds){
    if(!is_done() && cloud == clouds){
        if (text.length() > 0) {
            if(Collision::check_rect(box*Game_Manager::camera_zoom,Game_Manager::camera)){
                Bitmap_Font* font=Object_Manager::get_font("standard");

                font->show(box.x*Game_Manager::camera_zoom-Game_Manager::camera.x,box.y*Game_Manager::camera_zoom-Game_Manager::camera.y,text,color,opacity,scale,scale,angle);
            }
        }
        else if(!line){
            if(Collision::check_rect(box*Game_Manager::camera_zoom,Game_Manager::camera)){
                if (cast_shadow) {
                    sprite.render((box.x + Game_Constants::SHADOW_OFFSET)*Game_Manager::camera_zoom-Game_Manager::camera.x,(box.y + Game_Constants::SHADOW_OFFSET)*Game_Manager::camera_zoom-Game_Manager::camera.y,0.25,scale*0.65,scale*0.65,angle,"ui_black");
                }

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
