/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "../game/game.h"

#include <ui/special_info.h>
#include <log.h>
#include <data/object_manager.h>
#include <strings/engine_strings.h>
#include <data/engine_data.h>
#include <engine.h>
#include <engine_version.h>

using namespace std;

string Special_Info::get_special_info_text (string special_info) {
    string text = "";

    if (special_info.length() > 0) {
        if (special_info == "configure_command") {
            Object_Manager::output_command_configuration_info(text);
        } else if (special_info == "about") {
            text += "Cosmic Runner\nDeveloped by: " + Engine_Data::developer + "\nVersion: " +
                    Engine_Version::get_version() + " " + Engine_Version::get_status() + "\nBuilt on: " +
                    Engine_Version::get_build_date() + "\nChecksum: " + Engine::CHECKSUM + "\nEngine version: " +
                    Engine_Version::get_engine_version() + " " + Engine_Version::get_engine_status() + " (updated on " +
                    Engine_Version::get_engine_date() + ")";
        } else if (special_info == "changelog") {
            text += Engine::get_changelog();
        } else if (special_info == "game_over") {
            text += "Your career has come to an abrupt end.\nYou scored " + Strings::num_to_string(Game::get_score()) +
                    " points.\nCause of death:\n" + Game::get_cause_of_death();

            if (Game::is_score_high()) {
                text += "\n\nYou've made it onto the local\nhigh score table!";
            }
        } else if (special_info == "high_scores") {
            vector<High_Score> high_scores = Game::get_high_scores();

            for (const auto high_score : high_scores) {
                string space = "                            ";

                for (size_t i = 0; i < high_score.get_name().length(); i++) {
                    space.pop_back();
                }

                text += high_score.get_name() + space + Strings::num_to_string(high_score.get_score()) + "\n";
            }
        } else {
            Log::add_error("Invalid special info text: '" + special_info + "'");
        }
    }

    return text;
}

string Special_Info::get_special_info_sprite (string special_info) {
    string str_sprite_name = "";

    if (special_info.length() > 0) {
        if (special_info == "example") {
            str_sprite_name = "";
        } else {
            Log::add_error("Invalid special info sprite: '" + special_info + "'");
        }
    }

    return str_sprite_name;
}
