/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "planet.h"

#include <game_manager.h>
///QQQ includes
///#include <render.h>
///

using namespace std;

Planet::Planet(string new_sprite,const Coords<double>& position){
    sprite.set_name(new_sprite);

    circle.x=position.x;
    circle.y=position.y;
    circle.r=sprite.get_width()/2.0;
}

Collision_Circ<double> Planet::get_circle() const{
    return circle;
}

void Planet::animate(){
    sprite.animate();
}

void Planet::render(){
    if(Collision::check_circ_rect(circle*Game_Manager::camera_zoom,Game_Manager::camera)){
        sprite.render((circle.x-circle.r)*Game_Manager::camera_zoom-Game_Manager::camera.x,(circle.y-circle.r)*Game_Manager::camera_zoom-Game_Manager::camera.y);

        ///QQQ render collision circle
        ///Render::render_circle(circle.x*Game_Manager::camera_zoom-Game_Manager::camera.x,circle.y*Game_Manager::camera_zoom-Game_Manager::camera.y,circle.r,0.25,"red");
        ///
    }
}
