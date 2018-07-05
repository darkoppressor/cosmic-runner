/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef android_leaderboard_h
#define android_leaderboard_h

#include <window.h>

#include <cstdint>
#include <string>
#include <set>

class Android_Leaderboard {
private:

    static void save_failed_submission(std::uint32_t id_number);
    static std::set<std::uint32_t> load_failed_submissions();

public:

    //Returns the leaderboard id for the passed id number if it exists
    //Returns an empty string if the leaderboard id does not exist
    static std::string get_leaderboard_id(std::uint32_t id_number);

    //Submit a highscore for the overall leaderboard
    static void submit_highscore(std::uint64_t score);

    static void check_for_failed_submissions();

    static void update_sign_in_button(Window* window);

    static void update_leaderboards_button(Window* window);

    static void remove_android_buttons_from_window(Window* window);
    static void remove_android_buttons();

    static void update_windows();
};

#endif