/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "debris.h"
#include "game_data.h"

#include <game_manager.h>
///QQQ includes
///#include <render.h>

using namespace std;

Debris::Debris(string new_type,const Coords<double>& position,double new_angle,const Vector& new_angular_velocity){
    type=new_type;

    box.x=position.x;
    box.y=position.y;

    angular_velocity=new_angular_velocity;

    angle=new_angle;

    sprite.set_name(get_debris_type()->sprite);

    box.w=sprite.get_width();
    box.h=sprite.get_height();
}

Debris_Type* Debris::get_debris_type() const{
    return Game_Data::get_debris_type(type);
}

Collision_Rect<double> Debris::get_box() const{
    return box;
}

Collision_Rect<double> Debris::get_collision_box() const{
    return Collision_Rect<double>(box.x+box.w/2.0-box.w*get_debris_type()->collision_percentage/2.0,box.y+box.h/2.0-box.h*get_debris_type()->collision_percentage/2.0,
                                  box.w*get_debris_type()->collision_percentage,box.h*get_debris_type()->collision_percentage);
}

void Debris::rotation(){
    if(angular_velocity.direction>=0 && angular_velocity.direction<180){
        angle+=angular_velocity.magnitude;
    }
    else{
        angle-=angular_velocity.magnitude;
    }
}

void Debris::animate(){
    sprite.animate();
}

void Debris::render(){
    if(Collision::check_rect(box*Game_Manager::camera_zoom,Game_Manager::camera)){
        sprite.render(box.x*Game_Manager::camera_zoom-Game_Manager::camera.x,box.y*Game_Manager::camera_zoom-Game_Manager::camera.y,1.0,1.0,1.0,angle);

        ///QQQ render collision boxes
        /**Collision_Rect<double> col_box=get_collision_box();
        Render::render_rectangle(box.x*Game_Manager::camera_zoom-Game_Manager::camera.x,box.y*Game_Manager::camera_zoom-Game_Manager::camera.y,box.w,box.h,0.5,"ui_white");
        Render::render_rectangle(col_box.x*Game_Manager::camera_zoom-Game_Manager::camera.x,col_box.y*Game_Manager::camera_zoom-Game_Manager::camera.y,col_box.w,col_box.h,0.5,"red");*/
        ///
    }
}
