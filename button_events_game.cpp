/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game.h"

#include <button_events.h>
#include <window_manager.h>
#include <game_manager.h>

#include <boost/algorithm/string.hpp>

using namespace std;

bool Button_Events::handle_button_event_game(string button_event,Window* parent_window,bool& window_opened_on_top){
    if(button_event=="game_over"){
        Window_Manager::close_all_windows();

        Game_Manager::stop();

        Window_Manager::get_window("main_menu")->toggle_on();

        window_opened_on_top=true;

        return true;
    }
    else if(boost::algorithm::starts_with(button_event,"select_upgrade_")){
        boost::algorithm::erase_first(button_event,"select_upgrade_");

        Window_Manager::close_all_windows();

        Game::player_add_upgrade(button_event);

        Game::complete_contract();

        return true;
    }
    else if(button_event=="skip_upgrade"){
        Window_Manager::close_all_windows();

        Game::complete_contract();

        return true;
    }

    return false;
}
