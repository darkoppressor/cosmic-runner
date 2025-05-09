/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "../game/game.h"
#include "../android/android_leaderboard.h"

#include <ui/button_events.h>
#include <ui/window_manager.h>
#include <game/game_manager.h>
#include <android/android.h>
#include <ui/gui_manager.h>

#include <boost/algorithm/string.hpp>

using namespace std;

bool Button_Events::handle_button_event_game (string button_event, Window* parent_window, bool& window_opened_on_top) {
    if (button_event == "game_over") {
        Window_Manager::close_all_windows();

        if (Game::is_score_high()) {
            Window_Manager::get_window("input_name")->toggle_on();

            GUI_Manager::confirm_gui_object();
        } else {
            Android_Leaderboard::submit_highscore(Android_Leaderboard::HIGH_SCORES, Game::get_score());
            Android_Leaderboard::submit_highscore(Android_Leaderboard::BEST_KILL_COUNT, Game::get_kills());
            Android_Leaderboard::submit_highscore(Android_Leaderboard::DEBRIS_DODGED, Game::get_dodges());

            Game_Manager::stop();

            Window_Manager::get_window("main_menu")->toggle_on();

            Window_Manager::get_window("high_scores")->toggle_on();
        }

        window_opened_on_top = true;

        return true;
    } else if (button_event == "gpg_toggle_sign_in") {
        if (Android::gpg_is_silent_sign_in_attempt_complete()) {
            if (!Android::gpg_is_signed_in()) {
                Android::gpg_sign_in();

                Game::android_gpg_signing_in();
            } else {
                Android::gpg_sign_out();
            }
        }

        return true;
    } else if (boost::algorithm::starts_with(button_event, "gpg_show_leaderboard_")) {
        boost::algorithm::erase_first(button_event, "gpg_show_leaderboard_");

        if (Android::gpg_is_signed_in()) {
            Android::gpg_show_leaderboard(button_event);
        }

        return true;
    } else if (button_event == "gpg_show_all_leaderboards") {
        if (Android::gpg_is_signed_in()) {
            Android::gpg_show_all_leaderboards();
        }

        return true;
    } else if (button_event == "gpg_show_achievements") {
        if (Android::gpg_is_signed_in()) {
            Android::gpg_show_achievements();
        }

        return true;
    } else if (button_event == "name") {
        Window_Manager::close_all_windows();

        if (parent_window != 0) {
            Game::add_high_score(parent_window->get_info_text(0));

            Android_Leaderboard::submit_highscore(Android_Leaderboard::HIGH_SCORES, Game::get_score());
            Android_Leaderboard::submit_highscore(Android_Leaderboard::BEST_KILL_COUNT, Game::get_kills());
            Android_Leaderboard::submit_highscore(Android_Leaderboard::DEBRIS_DODGED, Game::get_dodges());
        }

        Game_Manager::stop();

        Window_Manager::get_window("main_menu")->toggle_on();

        Window_Manager::get_window("high_scores")->toggle_on();

        window_opened_on_top = true;

        return true;
    } else if (boost::algorithm::starts_with(button_event, "select_upgrade_")) {
        boost::algorithm::erase_first(button_event, "select_upgrade_");

        Window_Manager::close_all_windows();

        Game::player_add_upgrade(button_event);

        Game::complete_contract();

        return true;
    } else if (button_event == "skip_upgrade") {
        Window_Manager::close_all_windows();

        Game::restore_hull_from_contract();

        Game::complete_contract();

        return true;
    }

    return false;
}
