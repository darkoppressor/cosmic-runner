/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "debris.h"
#include "game_data.h"
#include "game_options.h"

#include <game_manager.h>
#include <render.h>

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

double Debris::get_angle() const{
    return angle;
}

Vector Debris::get_angular_velocity() const{
    return angular_velocity;
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
    if(Collision::check_rect_rotated(box*Game_Manager::camera_zoom,Game_Manager::camera,angle,0.0)){
        sprite.render(box.x*Game_Manager::camera_zoom-Game_Manager::camera.x,box.y*Game_Manager::camera_zoom-Game_Manager::camera.y,1.0,1.0,1.0,angle);

        if(Game_Options::show_collision_outlines){
            Collision_Rect<double> col_box=get_collision_box();

            /// Render extra collision boxes
            /*Render::render_rectangle(box.x*Game_Manager::camera_zoom-Game_Manager::camera.x,box.y*Game_Manager::camera_zoom-Game_Manager::camera.y,box.w,box.h,0.25,"white");
            Render::render_rectangle(col_box.x*Game_Manager::camera_zoom-Game_Manager::camera.x,col_box.y*Game_Manager::camera_zoom-Game_Manager::camera.y,col_box.w,col_box.h,0.25,"red");*/

            vector<Coords<double>> vertices;
            col_box.get_vertices(vertices,angle);

            for(size_t i=0;i<vertices.size();i++){
                uint32_t start_vertex=(uint32_t)i;
                uint32_t end_vertex=(i<vertices.size()-1) ? start_vertex+1 : 0;

                Render::render_line(vertices[start_vertex].x*Game_Manager::camera_zoom-Game_Manager::camera.x,vertices[start_vertex].y*Game_Manager::camera_zoom-Game_Manager::camera.y,
                                    vertices[end_vertex].x*Game_Manager::camera_zoom-Game_Manager::camera.x,vertices[end_vertex].y*Game_Manager::camera_zoom-Game_Manager::camera.y,1.0,"red");
            }
        }
    }
}
