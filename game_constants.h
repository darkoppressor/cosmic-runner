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
    static std::uint64_t POINT_VALUE_CONTRACT;

    static std::uint32_t UPGRADE_LIST_SIZE;

    static std::uint32_t EFFECT_LENGTH_CARGO;

    static std::uint32_t SHIELD_RECHARGE_RATE;

    static std::uint32_t WEAPON_SPREAD_DISTANCE;

    static double DESPAWN_DISTANCE;
    static std::uint32_t SHIP_SPAWN_DISTANCE_MAX;
    static std::uint32_t SHIP_SPAWN_DISTANCE_MIN;
    static std::uint32_t SHIP_SPAWN_RATE;

    static double MISSILE_HOMING_RANGE;

    static std::int32_t EXPLOSION_DAMAGE_SHIP;
};

#endif
