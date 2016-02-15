/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef game_constants_h
#define game_constants_h

#include <cstdint>

class Game_Constants{
public:

    static std::uint32_t TITLE_SHIP_SPAWN_RATE;
    static std::uint32_t TITLE_MAX_SHIPS;

    static std::uint32_t MAX_BACKGROUND_STAR_SIZE;
    static std::uint32_t BASE_BACKGROUND_STARS;

    static std::uint32_t COLLISION_CHANCE_DEBRIS;

    static std::uint64_t SCORE_MULTIPLIER_INCREASE;
    static std::uint64_t SCORE_MULTIPLIER_DECREASE;
    static std::uint64_t POINT_VALUE_CONTRACT;

    static std::uint32_t UPGRADE_LIST_SIZE;

    static std::uint32_t EFFECT_LENGTH_CARGO;

    static std::uint32_t SHIELD_RECHARGE_RATE;
    static std::uint32_t DISABLED_LENGTH;
    static std::uint32_t MAX_POWER;

    static std::int32_t ITEM_RESTORE_HULL;
    static std::uint32_t ITEM_RESTORE_POWER;

    static double ITEM_START_VELOCITY_MIN;
    static double ITEM_START_VELOCITY_MAX;
    static double ITEM_MAX_SPEED;

    static double SHIP_MANEUVER_ANGLE;

    static double POLICE_LIGHTS_ANGULAR_SPEED;

    static double POINT_DEFENSE_RANGE;

    static double ITEM_VACUUM_RANGE;
    static double ITEM_VACUUM_FORCE;

    static double STAR_RADIUS;
    static double STAR_DAMAGE_RANGE;
    static std::uint32_t STAR_DAMAGE_RATE;
    static std::int32_t STAR_DAMAGE;
    static double MINIMUM_GEN_DISTANCE_FROM_STAR;

    static double GEN_PLAYER_START_CLEAN_DISTANCE;

    static std::uint32_t NOTORIETY_MAX;
    static std::uint32_t NOTORIETY_TIER_2;
    static std::uint32_t NOTORIETY_TIER_1;
    static std::uint32_t NOTORIETY_INCREASE;

    static std::uint32_t WEAPON_SPREAD_DISTANCE;

    static double DESPAWN_DISTANCE;
    static std::uint32_t SPAWN_DISTANCE_MAX;
    static std::uint32_t SPAWN_DISTANCE_MIN;
    static std::uint32_t SHIP_SPAWN_RATE;
    static std::uint32_t ITEM_SPAWN_RATE;

    static std::uint32_t MAX_ATTEMPTS_SPAWN;

    static double AI_RANGE;
    static double AI_FOLLOW_DISTANCE;
    static double AI_PATROL_DISTANCE;
    static double MISSILE_HOMING_RANGE;

    static double TRACTOR_RANGE;
    static double TRACTOR_FORCE;

    static std::uint32_t MISSILE_HOMING_DELAY;

    static std::uint32_t AI_PROXIMITY_CHECK_PERIOD;

    static std::int32_t EXPLOSION_DAMAGE_SHIP;
};

#endif
