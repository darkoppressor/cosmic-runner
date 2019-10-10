/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef android_achievements_h
#define android_achievements_h

#include <window.h>

#include <cstdint>
#include <string>
#include <set>

class Android_Achievements {
    private:
        static void save_failed_submission(std::uint32_t id_number);
        static std::set<std::uint32_t> load_failed_submissions();

    public:
        static std::string FAILED_SUBMISSIONS_FILE;
        static std::uint32_t COP_KILLER_TIER_1;
        static std::uint32_t COP_KILLER_TIER_2;
        static std::uint32_t COP_KILLER_TIER_3;
        static std::uint32_t COP_KILLER_TIER_4;
        static std::uint32_t PIRATE_TIER_1;
        static std::uint32_t PIRATE_TIER_2;
        static std::uint32_t PIRATE_TIER_3;
        static std::uint32_t PIRATE_TIER_4;
        static std::uint32_t PIRATE_HUNTER_TIER_1;
        static std::uint32_t PIRATE_HUNTER_TIER_2;
        static std::uint32_t PIRATE_HUNTER_TIER_3;
        static std::uint32_t PIRATE_HUNTER_TIER_4;
        static std::uint32_t SURVIVALIST_TIER_1;
        static std::uint32_t SURVIVALIST_TIER_2;
        static std::uint32_t SURVIVALIST_TIER_3;
        static std::uint32_t SURVIVALIST_TIER_4;
        static std::uint32_t BIG_SCORER;
        static std::uint32_t ALL_THE_THINGS;
        static std::uint32_t ARTFUL_TIER_1;
        static std::uint32_t ARTFUL_TIER_2;
        static std::uint32_t ARTFUL_TIER_3;
        static std::uint32_t ARTFUL_TIER_4;
        static std::uint32_t ARTFUL_TIER_5;

        // Returns the achievement id for the passed id number if it exists
        // Returns an empty string if the achievement id does not exist
        static std::string get_achievement_id(std::uint32_t id_number);

        // Unlock the achievement with the passed id number
        static void unlock(std::uint32_t id_number);
        static void check_for_failed_submissions();
        static void update_achievements_button(Window* window);
        static void update_windows();
};

#endif
