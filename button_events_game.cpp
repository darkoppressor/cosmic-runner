/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game.h"
#include "android_leaderboard.h"

#include <button_events.h>
#include <window_manager.h>
#include <game_manager.h>
#include <android.h>

#include <boost/algorithm/string.hpp>

using namespace std;

bool Button_Events::handle_button_event_game(string button_event,Window* parent_window,bool& window_opened_on_top){
    if (button_event=="game_over") {
        Window_Manager::close_all_windows();

        if (Game::is_score_high()) {
            Window_Manager::get_window("input_name")->toggle_on();
        } else {
            Android_Leaderboard::submit_highscore(0, Game::get_score());
            Android_Leaderboard::submit_highscore(1, Game::get_kills());

            Game_Manager::stop();

            Window_Manager::get_window("main_menu")->toggle_on();

            Window_Manager::get_window("high_scores")->toggle_on();
        }

        window_opened_on_top=true;

        return true;
    } else if (button_event=="gpg_toggle_sign_in") {
        if(Android::gpg_is_silent_sign_in_attempt_complete()){
            if(!Android::gpg_is_signed_in()){
                Android::gpg_sign_in();

                Game::android_gpg_signing_in();
            }
            else{
                Android::gpg_sign_out();
            }
        }

        return true;
    } else if (boost::algorithm::starts_with(button_event,"gpg_show_leaderboard_")) {
        boost::algorithm::erase_first(button_event,"gpg_show_leaderboard_");

        if(Android::gpg_is_signed_in()){
            Android::gpg_show_leaderboard(button_event.c_str());
        }

        return true;
    } else if (button_event=="gpg_show_all_leaderboards") {
        if(Android::gpg_is_signed_in()){
            Android::gpg_show_all_leaderboards();
        }

        return true;
    } else if (button_event=="name") {
        Window_Manager::close_all_windows();

        if (parent_window != 0) {
            Game::add_high_score(parent_window->get_info_text(0));

            Android_Leaderboard::submit_highscore(0, Game::get_score());
            Android_Leaderboard::submit_highscore(1, Game::get_kills());
        }

        Game_Manager::stop();

        Window_Manager::get_window("main_menu")->toggle_on();

        Window_Manager::get_window("high_scores")->toggle_on();

        window_opened_on_top=true;

        return true;
    } else if (boost::algorithm::starts_with(button_event,"select_upgrade_")) {
        boost::algorithm::erase_first(button_event,"select_upgrade_");

        Window_Manager::close_all_windows();

        Game::player_add_upgrade(button_event);

        Game::complete_contract();

        return true;
    } else if (button_event=="skip_upgrade") {
        Window_Manager::close_all_windows();

        Game::complete_contract();

        return true;
    }

    return false;
}
