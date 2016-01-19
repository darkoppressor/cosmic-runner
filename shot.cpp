/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "shot.h"
#include "game_data.h"
#include "game.h"

#include <engine.h>
#include <game_manager.h>
///QQQ includes
///#include <render.h>

using namespace std;

Shot::Shot(string new_type,const Coords<double>& position,double new_angle){
    type=new_type;

    box.x=position.x;
    box.y=position.y;

    angle=new_angle;

    sprite.set_name(get_shot_type()->sprite);

    box.w=sprite.get_width();
    box.h=sprite.get_height();

    alive=true;
}

Shot_Type* Shot::get_shot_type() const{
    return Game_Data::get_shot_type(type);
}

Collision_Rect<double> Shot::get_box() const{
    return box;
}

Vector Shot::get_velocity() const{
    return velocity;
}

double Shot::get_thrust_accel() const{
    return get_shot_type()->thrust_accel;
}

double Shot::get_max_speed() const{
    return get_shot_type()->max_speed;
}

Collision_Rect<double> Shot::get_collision_box() const{
    return Collision_Rect<double>(box.x+box.w/2.0-box.w*get_shot_type()->collision_percentage/2.0,box.y+box.h/2.0-box.h*get_shot_type()->collision_percentage/2.0,
                                  box.w*get_shot_type()->collision_percentage,box.h*get_shot_type()->collision_percentage);
}

bool Shot::is_alive() const{
    return alive;
}

void Shot::die(){
    if(is_alive()){
        alive=false;

        ///QQQ create death effect
    }
}

void Shot::thrust(){
    Vector thrust_force(get_thrust_accel(),angle);

    force+=thrust_force;
}

void Shot::accelerate(){
    if(is_alive()){
        thrust();

        Vector acceleration=force/get_shot_type()->mass;

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

void Shot::movement(const Quadtree<double,uint32_t>& quadtree_debris){
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

            if(Collision::check_rect(box_collision,box_debris)){
                die();
            }
        }
    }
}

void Shot::animate(){
    if(is_alive()){
        sprite.animate();
    }
}

void Shot::render(){
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
