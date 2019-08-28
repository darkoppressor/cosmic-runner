/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "android_leaderboard.h"
#include "game_constants.h"
#include "game.h"
#include "game_data.h"

#include <file_io.h>
#include <android.h>
#include <directories.h>
#include <engine_strings.h>
#include <window_manager.h>

#include <boost/algorithm/string.hpp>

using namespace std;

string Android_Leaderboard::FAILED_SUBMISSIONS_FILE = "android_leaderboard_submissions";
uint32_t Android_Leaderboard::HIGH_SCORES = 0;
uint32_t Android_Leaderboard::BEST_KILL_COUNT = 1;
uint32_t Android_Leaderboard::DEBRIS_DODGED = 2;
void Android_Leaderboard::save_failed_submission (uint32_t id_number, uint64_t score) {
    map<uint32_t, uint64_t> failed_submissions = load_failed_submissions();

    if (!failed_submissions.count(id_number)) {
        failed_submissions.emplace(id_number, score);

        string data = "";

        for (const auto& failed_submission : failed_submissions) {
            data += Strings::num_to_string(failed_submission.first) + "," + Strings::num_to_string(
                failed_submission.second) + "\n";
        }

        File_IO::save_atomic(Directories::get_save_directory() + FAILED_SUBMISSIONS_FILE, data);
    }
}

map<uint32_t, uint64_t> Android_Leaderboard::load_failed_submissions () {
    map<uint32_t, uint64_t> failed_submissions;
    string file_name = Directories::get_save_directory() + FAILED_SUBMISSIONS_FILE;

    if (File_IO::exists(file_name) && File_IO::is_regular_file(file_name)) {
        File_IO_Load load(file_name);

        if (load.is_opened()) {
            while (!load.eof()) {
                string line = "";
                load.getline(&line);

                vector<string> elements;
                boost::algorithm::split(elements, line, boost::algorithm::is_any_of(","));

                if (elements.size() >= 2) {
                    failed_submissions.emplace((uint32_t) Strings::string_to_unsigned_long(
                                                   elements[0]), (uint64_t) Strings::string_to_unsigned_long(
                                                   elements[1]));
                }
            }
        }
    }

    return failed_submissions;
}

string Android_Leaderboard::get_leaderboard_id (uint32_t id_number) {
    File_IO_Load load(VFS::get_rwops("android_leaderboard_ids"));

    if (load.is_opened()) {
        for (uint32_t i = 0; !load.eof(); i++) {
            string line = "";
            load.getline(&line);

            if (i == id_number) {
                return line;
            }
        }
    }

    return "";
}

void Android_Leaderboard::submit_highscore (uint32_t id_number, uint64_t score) {
    // Retrieve the leaderboard id
    string id = get_leaderboard_id(id_number);

    if (id.length() > 0) {
        if (Android::gpg_is_signed_in()) {
            Android::gpg_submit_highscore(id, score);
        } else {
            save_failed_submission(id_number, score);
        }
    }
}

void Android_Leaderboard::check_for_failed_submissions () {
    map<uint32_t, uint64_t> failed_submissions = load_failed_submissions();

    if (File_IO::exists(Directories::get_save_directory() + FAILED_SUBMISSIONS_FILE)) {
        File_IO::remove_file(Directories::get_save_directory() + FAILED_SUBMISSIONS_FILE);
    }

    for (const auto& failed_submission : failed_submissions) {
        submit_highscore(failed_submission.first, failed_submission.second);
    }
}

void Android_Leaderboard::update_sign_in_button (Window* window) {
    #ifdef GAME_OS_ANDROID

        if (Android::gpg_is_silent_sign_in_attempt_complete()) {
            if (!Android::gpg_is_signed_in()) {
                window->informations.back().set_sprite("play_games_controller_gray");
                window->buttons.back().set_text("Sign In");
            } else {
                window->informations.back().set_sprite("play_games_controller_green");
                window->buttons.back().set_text("Sign Out");
            }
        } else {
            window->informations.back().set_sprite("play_games_controller_gray");
            window->buttons.back().set_text("Sign In");
        }

    #endif
}

void Android_Leaderboard::update_leaderboards_button (Window* window) {
    #ifdef GAME_OS_ANDROID

        if (Android::gpg_is_silent_sign_in_attempt_complete()) {
            if (!Android::gpg_is_signed_in()) {
                window->informations[window->informations.size() - 2].set_sprite("play_games_leaderboards_gray");
            } else {
                window->informations[window->informations.size() - 2].set_sprite("play_games_leaderboards_green");
            }
        } else {
            window->informations[window->informations.size() - 2].set_sprite("play_games_leaderboards_gray");
        }

    #endif
}

void Android_Leaderboard::remove_android_buttons_from_window (Window* window, size_t buttons_to_remove) {
    #ifndef GAME_OS_ANDROID

        for (size_t i = 0; i < buttons_to_remove; i++) {
            window->informations.pop_back();
            window->buttons.pop_back();
        }

    #endif
}

void Android_Leaderboard::remove_android_buttons () {
    #ifndef GAME_OS_ANDROID
        remove_android_buttons_from_window(Window_Manager::get_window("main_menu"), 1);
        remove_android_buttons_from_window(Window_Manager::get_window("ingame_menu"), 1);
        remove_android_buttons_from_window(Window_Manager::get_window("high_scores"), 2);
    #endif
}

void Android_Leaderboard::update_windows () {
    update_sign_in_button(Window_Manager::get_window("main_menu"));
    update_sign_in_button(Window_Manager::get_window("ingame_menu"));
    update_leaderboards_button(Window_Manager::get_window("high_scores"));
}
