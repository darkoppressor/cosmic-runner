/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game.h"

#include <engine.h>
#include <game_manager.h>
#include <font.h>
#include <object_manager.h>
#include <options.h>
#include <network_engine.h>
#include <render.h>
#include <engine_strings.h>
#include <symbols.h>

using namespace std;

void Engine::render_dev_info(){
    string msg="";

    if(Game_Manager::in_progress){
        msg+="Camera Position: "+Strings::num_to_string(Game_Manager::camera.x)+","+Strings::num_to_string(Game_Manager::camera.y)+"\n";
        msg+="Camera Size: "+Strings::num_to_string(Game_Manager::camera.w/Game_Manager::camera_zoom)+","+Strings::num_to_string(Game_Manager::camera.h/Game_Manager::camera_zoom)+"\n";
        msg+="Camera Zoom: "+Strings::num_to_string(Game_Manager::camera_zoom)+"\n";

        msg+="\n-----Game-----\n";
        msg+="Score: "+Strings::num_to_string(Game::get_score())+" (x"+Strings::num_to_string(Game::get_score_multiplier())+" multiplier)\n";

        msg+="\n-----Universe-----\n";
        msg+="Ships: "+Strings::num_to_string(Game::get_ship_count())+"\n";
        msg+="Debris: "+Strings::num_to_string(Game::get_debris_count())+"\n";
        msg+="Planets: "+Strings::num_to_string(Game::get_planet_count())+"\n";

        msg+="\n-----Ship-----\n";
        msg+="Position: "+Strings::num_to_string(Game::get_player_const().get_box().center_x())+" , "+Strings::num_to_string(Game::get_player_const().get_box().center_y())+"\n";
        msg+="Velocity: "+Strings::num_to_string(Game::get_player_const().get_velocity().magnitude)+" m/s , "+Strings::num_to_string(Game::get_player_const().get_velocity().direction)+Symbols::degrees()+"\n";
        msg+="Hull: "+Strings::num_to_string(Game::get_player_const().get_hull())+"/"+Strings::num_to_string(Game::get_player_const().get_hull_max())+"\n";
        msg+="Shields: "+Strings::num_to_string(Game::get_player_const().get_shields())+"/"+Strings::num_to_string(Game::get_player_const().get_shields_max())+"\n";
    }

    if(msg.length()>0){
        Bitmap_Font* font=Object_Manager::get_font("small");

		double y=2.0;
        if(Options::fps){
            y+=font->spacing_y;

            if(Network_Engine::status!="off"){
                y+=font->spacing_y*2.0;
            }
        }

        Render::render_rectangle(2.0,y,Strings::longest_line(msg)*font->spacing_x,Strings::newline_count(msg)*font->spacing_y,0.75,"ui_black");
        font->show(2.0,y,msg,"red");
    }
}
