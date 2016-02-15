/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "title_ship.h"

#include <engine.h>
#include <game_window.h>

using namespace std;

Title_Ship::Title_Ship(string new_sprite,const Coords<double>& position,const Vector& new_velocity,double new_distance_scale){
    box.x=position.x;
    box.y=position.y;

    sprite.set_name(new_sprite);

    box.w=sprite.get_width();
    box.h=sprite.get_height();

    velocity=new_velocity;

    distance_scale=new_distance_scale;
}

bool Title_Ship::is_alive(){
    Vector_Components vc=velocity.get_components();

    if(vc.a<=0.0 && box.x+box.w<0.0){
        return false;
    }
    else if(vc.a>0.0 && box.x>Game_Window::width()-1.0){
        return false;
    }
    else{
        return true;
    }
}

void Title_Ship::movement(){
    Vector_Components vc=velocity.get_components();

    box.x+=vc.a/(double)Engine::UPDATE_RATE;
    box.y+=vc.b/(double)Engine::UPDATE_RATE;
}

void Title_Ship::animate(){
    sprite.animate();
}

void Title_Ship::render(){
    Vector_Components vc=velocity.get_components();

    bool flip_x=false;
    if(vc.a<=0.0){
        flip_x=true;
    }

    sprite.render(box.x,box.y,1.0,distance_scale,distance_scale,0.0,"white",flip_x);
}
