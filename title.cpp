/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "title.h"
#include "game_constants.h"

#include <game_window.h>
#include <engine.h>
#include <engine_strings.h>
#include <render.h>
#include <image_manager.h>

using namespace std;

Title::Title(){
    ship_spawn_check=0;
}

void Title::clear_title(){
    ships.clear();
    suns.clear();

    ship_spawn_check=0;
}

void Title::setup(RNG& rng){
    Sprite sprite;
    sprite.set_name("title_sun");

    Coords<double> position((double)Game_Window::width()/4.0-sprite.get_width()/2.0,(double)Game_Window::height()-sprite.get_height()*0.9);

    suns.push_back(Title_Sun(sprite.name,position));

    generate_ship(rng);
}

void Title::generate_ship(RNG& rng){
    if(ships.size()<Game_Constants::TITLE_MAX_SHIPS){
        Sprite sprite;
        sprite.set_name("title_ship_"+Strings::num_to_string(rng.random_range(0,2)));

        double distance_scale=0.01*(double)rng.random_range(20,80);

        Coords<double> position;
        Vector velocity;
        velocity.magnitude=distance_scale*0.1*(double)rng.random_range(250,500);

        if(rng.random_range(0,99)<50){
            position.x=0.0-sprite.get_width()*distance_scale;
            velocity.direction=0.0;
        }
        else{
            position.x=(double)Game_Window::width();
            velocity.direction=180.0;
        }

        position.y=rng.random_range(0,(uint32_t)(Game_Window::height()-sprite.get_height()));

        ships.push_back(Title_Ship(sprite.name,position,velocity,distance_scale));
    }
}

void Title::movement(){
    for(size_t i=0;i<ships.size();i++){
        ships[i].movement();
    }
}

void Title::check_ships(RNG& rng){
    for(size_t i=0;i<ships.size();){
        if(!ships[i].is_alive()){
            ships.erase(ships.begin()+i);
        }
        else{
            i++;
        }
    }

    if(++ship_spawn_check>=Game_Constants::TITLE_SHIP_SPAWN_RATE*Engine::UPDATE_RATE){
        ship_spawn_check=0;

        generate_ship(rng);
    }
}

void Title::animate(RNG& rng){
    for(size_t i=0;i<suns.size();i++){
        suns[i].animate(rng);
    }

    for(size_t i=0;i<ships.size();i++){
        ships[i].animate();
    }
}

void Title::render(){
    Render::render_texture(0.0,0.0,Image_Manager::get_image("title_background_0"));

    for(size_t i=0;i<suns.size();i++){
        suns[i].render();
    }

    Render::render_texture(0.0,0.0,Image_Manager::get_image("title_background_1"));

    for(size_t i=0;i<ships.size();i++){
        ships[i].render();
    }
}
