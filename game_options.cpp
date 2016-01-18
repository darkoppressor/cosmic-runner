/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game_options.h"

#include <engine_strings.h>

using namespace std;

uint32_t Game_Options::backgrounds=0;

bool Game_Options::get_option(string name,string& value){
    if(name=="cl_backgrounds"){
        value=Strings::num_to_string(backgrounds);

        return true;
    }

    return false;
}

void Game_Options::set_option(string name,string value){
    if(name=="cl_backgrounds"){
        backgrounds=Strings::string_to_unsigned_long(value);
    }
}
