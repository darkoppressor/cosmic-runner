/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game_options.h"

#include <engine_strings.h>

using namespace std;

uint32_t Game_Options::background_layers_stars=0;
uint32_t Game_Options::background_layers_planetary=0;

bool Game_Options::show_collision_outlines=false;

double Game_Options::minimap_opacity=0.0;

bool Game_Options::get_option(string name,string& value){
    if(name=="cl_background_layers_stars"){
        value=Strings::num_to_string(background_layers_stars);

        return true;
    }
    else if(name=="cl_background_layers_planetary"){
        value=Strings::num_to_string(background_layers_planetary);

        return true;
    }
    else if(name=="cl_show_collision_outlines"){
        value=Strings::bool_to_string(show_collision_outlines);

        return true;
    }
    else if(name=="cl_minimap_opacity"){
        value=Strings::num_to_string(minimap_opacity);

        return true;
    }

    return false;
}

void Game_Options::set_option(string name,string value){
    if(name=="cl_background_layers_stars"){
        background_layers_stars=Strings::string_to_unsigned_long(value);
    }
    else if(name=="cl_background_layers_planetary"){
        background_layers_planetary=Strings::string_to_unsigned_long(value);
    }
    else if(name=="cl_show_collision_outlines"){
        show_collision_outlines=Strings::string_to_bool(value);
    }
    else if(name=="cl_minimap_opacity"){
        minimap_opacity=Strings::string_to_double(value);
    }
}
