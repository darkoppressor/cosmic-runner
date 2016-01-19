/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game_constants.h"

#include <game_constants_loader.h>
#include <engine_strings.h>
#include <engine_data.h>

using namespace std;

uint32_t Game_Constants::MAX_BACKGROUND_STAR_SIZE=0;
uint32_t Game_Constants::BASE_BACKGROUND_STARS=0;

uint32_t Game_Constants::COLLISION_CHANCE_DEBRIS=0;

uint64_t Game_Constants::SCORE_MULTIPLIER_INCREASE=0;

uint32_t Game_Constants::UPGRADE_LIST_SIZE=0;

void Game_Constants_Loader::set_game_constant(string name,string value){
    if(name=="zoom_rate"){
        Engine_Data::ZOOM_RATE=Strings::string_to_double(value);
    }
    else if(name=="zoom_min"){
        Engine_Data::ZOOM_MIN=Strings::string_to_double(value);
    }
    else if(name=="zoom_max"){
        Engine_Data::ZOOM_MAX=Strings::string_to_double(value);
    }

    else if(name=="max_background_star_size"){
        Game_Constants::MAX_BACKGROUND_STAR_SIZE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="base_background_stars"){
        Game_Constants::BASE_BACKGROUND_STARS=Strings::string_to_unsigned_long(value);
    }

    else if(name=="collision_chance_debris"){
        Game_Constants::COLLISION_CHANCE_DEBRIS=Strings::string_to_unsigned_long(value);
    }

    else if(name=="score_multiplier_increase"){
        Game_Constants::SCORE_MULTIPLIER_INCREASE=Strings::string_to_unsigned_long(value);
    }

    else if(name=="upgrade_list_size"){
        Game_Constants::UPGRADE_LIST_SIZE=Strings::string_to_unsigned_long(value);
    }
}
