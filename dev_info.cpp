/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game.h"
#include "game_constants.h"

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
        msg+="Kills: "+Strings::num_to_string(Game::get_kills())+"\n";
        msg+="Power: "+Strings::num_to_string(Game::get_power())+"/"+Strings::num_to_string(Game_Constants::MAX_POWER*Engine::UPDATE_RATE);
        msg+=" ("+Strings::num_to_string(Game::get_power()/Engine::UPDATE_RATE)+" seconds)\n";
        if(Game::notoriety_tier_2()){
            msg+="Notoriety: Tier 2";
        }
        else if(Game::notoriety_tier_1()){
            msg+="Notoriety: Tier 1";
        }
        else{
            msg+="Notoriety: Tier 0";
        }
        msg+=" ("+Strings::num_to_string(Game::get_notoriety())+"/"+Strings::num_to_string(Game_Constants::NOTORIETY_MAX*Engine::UPDATE_RATE);
        msg+=", "+Strings::num_to_string(Game::get_notoriety()/Engine::UPDATE_RATE)+" seconds)\n";

        msg+="\n-----Universe-----\n";
        msg+="Planets: "+Strings::num_to_string(Game::get_planet_count())+"\n";
        msg+="Debris: "+Strings::num_to_string(Game::get_debris_count())+"\n";
        msg+="Ships: "+Strings::num_to_string(Game::get_ship_count())+"\n";
        msg+="Items: "+Strings::num_to_string(Game::get_item_count())+"\n";

        msg+="\n-----Ship-----\n";
        msg+="Position: "+Strings::num_to_string(Game::get_player_const().get_box().center_x())+" , "+Strings::num_to_string(Game::get_player_const().get_box().center_y())+"\n";
        msg+="Velocity: "+Strings::num_to_string(Game::get_player_const().get_velocity().magnitude)+" m/s , "+Strings::num_to_string(Game::get_player_const().get_velocity().direction)+Symbols::degrees()+"\n";
        msg+="Acceleration: "+Strings::num_to_string(Game::get_player_acceleration().magnitude)+" m/s"+Symbols::squared()+" , "+Strings::num_to_string(Game::get_player_acceleration().direction)+Symbols::degrees()+"\n";
        msg+="Hull: "+Strings::num_to_string(Game::get_player_const().get_hull())+"/"+Strings::num_to_string(Game::get_player_const().get_hull_max())+"\n";
        msg+="Shields: "+Strings::num_to_string(Game::get_player_const().get_shields())+"/"+Strings::num_to_string(Game::get_player_const().get_shields_max())+"\n";
        if(Game::get_player_const().are_weapons_enabled()){
            msg+="Weapons Systems: Enabled\n";
        }
        else{
            msg+="Weapons Systems: Disabled\n";
        }
        vector<string> upgrades=Game::get_player_const().get_upgrades();
        msg+="Upgrades: "+Strings::num_to_string(upgrades.size())+"\n";
        for(size_t i=0;i<upgrades.size();i++){
            msg+="Upgrade "+Strings::num_to_string(i)+": "+upgrades[i]+"\n";
        }
    }

    if(msg.length()>0){
        Bitmap_Font* font=Object_Manager::get_font("standard");
        Bitmap_Font* font_large=Object_Manager::get_font("large");

		double y=2.0;
        if(Options::fps){
            y+=font_large->spacing_y;

            if(Network_Engine::status!="off"){
                y+=font->spacing_y*2.0;
            }
        }

        Render::render_rectangle(2.0,y,Strings::longest_line(msg)*font->spacing_x,Strings::newline_count(msg)*font->spacing_y,0.75,"ui_black");
        font->show(2.0,y,msg,"red");
    }
}
