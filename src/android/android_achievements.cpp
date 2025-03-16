/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "android_achievements.h"
#include "../game/game_constants.h"
#include "../game/game.h"
#include "../game/game_data.h"

#include <file/file_io.h>
#include <android/android.h>
#include <file/directories.h>
#include <strings/engine_strings.h>
#include <ui/window_manager.h>

#include <boost/algorithm/string.hpp>

using namespace std;

string Android_Achievements::FAILED_SUBMISSIONS_FILE = "android_achievement_submissions";
uint32_t Android_Achievements::COP_KILLER_TIER_1 = 0;
uint32_t Android_Achievements::COP_KILLER_TIER_2 = 1;
uint32_t Android_Achievements::COP_KILLER_TIER_3 = 2;
uint32_t Android_Achievements::COP_KILLER_TIER_4 = 3;
uint32_t Android_Achievements::PIRATE_TIER_1 = 4;
uint32_t Android_Achievements::PIRATE_TIER_2 = 5;
uint32_t Android_Achievements::PIRATE_TIER_3 = 6;
uint32_t Android_Achievements::PIRATE_TIER_4 = 7;
uint32_t Android_Achievements::PIRATE_HUNTER_TIER_1 = 8;
uint32_t Android_Achievements::PIRATE_HUNTER_TIER_2 = 9;
uint32_t Android_Achievements::PIRATE_HUNTER_TIER_3 = 10;
uint32_t Android_Achievements::PIRATE_HUNTER_TIER_4 = 11;
uint32_t Android_Achievements::SURVIVALIST_TIER_1 = 12;
uint32_t Android_Achievements::SURVIVALIST_TIER_2 = 13;
uint32_t Android_Achievements::SURVIVALIST_TIER_3 = 14;
uint32_t Android_Achievements::SURVIVALIST_TIER_4 = 15;
uint32_t Android_Achievements::BIG_SCORER = 16;
uint32_t Android_Achievements::ALL_THE_THINGS = 17;
uint32_t Android_Achievements::ARTFUL_TIER_1 = 18;
uint32_t Android_Achievements::ARTFUL_TIER_2 = 19;
uint32_t Android_Achievements::ARTFUL_TIER_3 = 20;
uint32_t Android_Achievements::ARTFUL_TIER_4 = 21;
uint32_t Android_Achievements::ARTFUL_TIER_5 = 22;

void Android_Achievements::save_failed_submission (uint32_t id_number) {
    set<uint32_t> failed_submissions = load_failed_submissions();

    if (!failed_submissions.count(id_number)) {
        failed_submissions.emplace(id_number);

        string data = "";

        for (const auto& failed_submission : failed_submissions) {
            data += Strings::num_to_string(failed_submission) + "\n";
        }

        File_IO::save_atomic(Directories::get_save_directory() + FAILED_SUBMISSIONS_FILE, data);
    }
}

set<uint32_t> Android_Achievements::load_failed_submissions () {
    set<uint32_t> failed_submissions;
    string file_name = Directories::get_save_directory() + FAILED_SUBMISSIONS_FILE;

    if (File_IO::exists(file_name) && File_IO::is_regular_file(file_name)) {
        File_IO_Load load(file_name);

        if (load.is_opened()) {
            while (!load.eof()) {
                string line = "";

                load.getline(&line);

                failed_submissions.emplace((uint32_t) Strings::string_to_unsigned_long(line));
            }
        }
    }

    return failed_submissions;
}

string Android_Achievements::get_achievement_id (uint32_t id_number) {
    File_IO_Load load(VFS::get_rwops("android_achievement_ids"));

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

void Android_Achievements::unlock (uint32_t id_number) {
    // Retrieve the achievement id
    string id = get_achievement_id(id_number);

    if (id.length() > 0) {
        if (Android::gpg_is_signed_in()) {
            Android::gpg_unlock_achievement(id);
        } else {
            save_failed_submission(id_number);
        }
    }
}

void Android_Achievements::check_for_failed_submissions () {
    set<uint32_t> failed_submissions = load_failed_submissions();

    if (File_IO::exists(Directories::get_save_directory() + FAILED_SUBMISSIONS_FILE)) {
        File_IO::remove_file(Directories::get_save_directory() + FAILED_SUBMISSIONS_FILE);
    }

    for (const auto& failed_submission : failed_submissions) {
        unlock(failed_submission);
    }
}

void Android_Achievements::update_achievements_button (Window* window) {
    #ifdef GAME_OS_ANDROID

        if (Android::gpg_is_silent_sign_in_attempt_complete()) {
            if (!Android::gpg_is_signed_in()) {
                window->informations.back().set_sprite("play_games_achievements_gray");
            } else {
                window->informations.back().set_sprite("play_games_achievements_green");
            }
        } else {
            window->informations.back().set_sprite("play_games_achievements_gray");
        }

    #endif
}

void Android_Achievements::update_windows () {
    update_achievements_button(Window_Manager::get_window("high_scores"));
}
