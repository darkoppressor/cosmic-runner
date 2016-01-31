/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "star.h"
#include "game_constants.h"

#include <game_manager.h>
#include <render.h>

using namespace std;

Star::Star(const Coords<double>& position){
    circle.x=position.x;
    circle.y=position.y;
    circle.r=Game_Constants::STAR_RADIUS;
}

Collision_Circ<double> Star::get_circle() const{
    return circle;
}

void Star::render(){
    if(Collision::check_circ_rect(circle*Game_Manager::camera_zoom,Game_Manager::camera)){
        Render::render_circle(circle.x*Game_Manager::camera_zoom-Game_Manager::camera.x,circle.y*Game_Manager::camera_zoom-Game_Manager::camera.y,circle.r,1.0,"star_yellow");

        ///QQQ render collision circle
        ///Render::render_circle(circle.x*Game_Manager::camera_zoom-Game_Manager::camera.x,circle.y*Game_Manager::camera_zoom-Game_Manager::camera.y,circle.r,0.5,"red");
        ///
    }
}
