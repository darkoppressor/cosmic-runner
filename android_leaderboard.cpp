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

using namespace std;

void Android_Leaderboard::save_failed_submission(uint32_t id_number){
    set<uint32_t> failed_submissions=load_failed_submissions();

    if(!failed_submissions.count(id_number)){
        failed_submissions.emplace(id_number);

        string file_name=Directories::get_save_directory();
        file_name+="leaderboard_submissions";

        string data="";

        for(const auto& i : failed_submissions){
            data+=Strings::num_to_string(i)+"\n";
        }

        File_IO::save_atomic(file_name,data);
    }
}

set<uint32_t> Android_Leaderboard::load_failed_submissions(){
    set<uint32_t> failed_submissions;

    string file_name=Directories::get_save_directory()+"leaderboard_submissions";

    if(File_IO::exists(file_name) && File_IO::is_regular_file(file_name)){
        File_IO_Load load(file_name);

        if(load.is_opened()){
            while(!load.eof()){
                string line="";
                load.getline(&line);

                failed_submissions.emplace((uint32_t)Strings::string_to_unsigned_long(line));
            }
        }
    }

    return failed_submissions;
}

string Android_Leaderboard::get_leaderboard_id(uint32_t id_number){
    File_IO_Load load(VFS::get_rwops("android_leaderboard_ids"));

    if(load.is_opened()){
        for(uint32_t i=0;!load.eof();i++){
            string line="";
            load.getline(&line);

            if(i==id_number){
                return line;
            }
        }
    }

    return "";
}

void Android_Leaderboard::submit_highscore (uint32_t id_number, uint64_t score) {
    //Retrieve the leaderboard id
    string id = get_leaderboard_id(id_number);

    if (id.length()>0) {
        if (Android::gpg_is_signed_in()) {
            Android::gpg_submit_highscore(id.c_str(), score);
        } else {
            save_failed_submission(id_number);
        }
    }
}

void Android_Leaderboard::check_for_failed_submissions(){
    set<uint32_t> failed_submissions=load_failed_submissions();

    if(File_IO::exists(Directories::get_save_directory()+"leaderboard_submissions")){
        File_IO::remove_file(Directories::get_save_directory()+"leaderboard_submissions");
    }

    submit_highscore(Game::get_score());
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
                window->informations.back().set_sprite("play_games_leaderboards_gray");
            } else {
                window->informations.back().set_sprite("play_games_leaderboards_green");
            }
        } else {
            window->informations.back().set_sprite("play_games_leaderboards_gray");
        }
    #endif
}

void Android_Leaderboard::remove_android_buttons_from_window (Window* window) {
    #ifndef GAME_OS_ANDROID
        window->informations.pop_back();
        window->buttons.pop_back();
    #endif
}

void Android_Leaderboard::remove_android_buttons () {
    #ifndef GAME_OS_ANDROID
        remove_android_buttons_from_window(Window_Manager::get_window("main_menu"));
        remove_android_buttons_from_window(Window_Manager::get_window("ingame_menu"));
        remove_android_buttons_from_window(Window_Manager::get_window("high_scores"));
    #endif
}

void Android_Leaderboard::update_windows () {
    update_sign_in_button(Window_Manager::get_window("main_menu"));
    update_sign_in_button(Window_Manager::get_window("ingame_menu"));
    update_leaderboards_button(Window_Manager::get_window("high_scores"));
}
