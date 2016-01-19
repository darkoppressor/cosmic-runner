/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef game_constants_h
#define game_constants_h

#include <cstdint>

class Game_Constants{
public:

    static std::uint32_t MAX_BACKGROUND_STAR_SIZE;
    static std::uint32_t BASE_BACKGROUND_STARS;

    static std::uint32_t COLLISION_CHANCE_DEBRIS;

    static std::uint64_t SCORE_MULTIPLIER_INCREASE;

    static std::uint32_t UPGRADE_LIST_SIZE;
};

#endif
