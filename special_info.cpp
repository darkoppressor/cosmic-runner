/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game.h"

#include <special_info.h>
#include <log.h>
#include <object_manager.h>
#include <engine_strings.h>

using namespace std;

string Special_Info::get_special_info_text(string special_info){
    string text="";

    if(special_info.length()>0){
        if (special_info=="configure_command") {
            Object_Manager::output_command_configuration_info(text);
        } else if (special_info=="game_over") {
            text+="Your career has come to an abrupt end.\nYou scored " + Strings::num_to_string(Game::get_score()) + " points.";

            if (Game::is_score_high()) {
                text+="\nYou've made it onto the local high score table!";
            }
        } else if (special_info=="high_scores") {
            vector<High_Score> high_scores = Game::get_high_scores();

            for (const auto high_score : high_scores) {
                string space = "                                                  ";

                for (size_t i = 0; i < high_score.get_name().length(); i++) {
                    space.pop_back();
                }

                text += "        " + high_score.get_name() + space + Strings::num_to_string(high_score.get_score()) + "\n";
            }
        } else {
            Log::add_error("Invalid special info text: '"+special_info+"'");
        }
    }

    return text;
}

string Special_Info::get_special_info_sprite(string special_info){
    string str_sprite_name="";

    if(special_info.length()>0){
        if(special_info=="example"){
            str_sprite_name="";
        }
        else{
            Log::add_error("Invalid special info sprite: '"+special_info+"'");
        }
    }

    return str_sprite_name;
}
