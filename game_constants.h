/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef game_constants_h
#define game_constants_h

#include <string>
#include <cstdint>

class Game_Constants{
public:

    /// BEGIN SCRIPT-GENERATED CONSTANT DECLARATIONS
    static std::uint32_t TITLE_SHIP_SPAWN_RATE;
    static std::uint32_t TITLE_MAX_SHIPS;
    static std::uint32_t TITLE_BIRD_CHANCE;
    static std::uint32_t TITLE_CLOUD_CHANCE;
    static std::uint32_t TITLE_SHIP_MINIMUM_HEIGHT;
    static std::uint32_t TITLE_MAX_SUN_SHIMMERS;
    static std::uint32_t TITLE_SUN_SHIMMER_SPAWN_RATE;
    static std::uint32_t TITLE_SUN_SHIMMER_SPAWN_CHANCE;
    static std::uint32_t TITLE_SUN_SHIMMER_Y_RIGHT_MOD_MIN;
    static std::uint32_t TITLE_SUN_SHIMMER_Y_RIGHT_MOD_MAX;
    static std::uint32_t TITLE_SUN_SHIMMER_SPEED_MIN;
    static std::uint32_t TITLE_SUN_SHIMMER_SPEED_MAX;
    static std::uint32_t TITLE_SUN_SHIMMER_RADIUS_MIN;
    static std::uint32_t TITLE_SUN_SHIMMER_RADIUS_MAX;
    static std::uint32_t MAX_BACKGROUND_STAR_SIZE;
    static std::uint32_t BASE_BACKGROUND_STARS;
    static std::uint32_t MAX_BACKGROUND_DEBRIS_SIZE;
    static std::uint32_t BASE_BACKGROUND_DEBRIS;
    static double BACKGROUND_FADE_RATE;
    static double HUD_SPACING;
    static double HUD_MAX_BAR_WIDTH;
    static double HUD_BAR_HEIGHT;
    static double HUD_BAR_OFFSET_X;
    static double HUD_BAR_OFFSET_Y;
    static double MINIMAP_BORDER_THICKNESS;
    static double MINIMAP_SHIP_SCALE;
    static double MINIMAP_ARROW_MAGNITUDE;
    static double MINIMAP_ARROW_ANGLE_MOD;
    static std::uint32_t MINIMAP_SIZE;
    static std::int32_t MINIMAP_LINE_THICKNESS;
    static std::uint32_t COLLISION_CHANCE_DEBRIS;
    static std::uint64_t SCORE_MULTIPLIER_INCREASE;
    static std::uint64_t SCORE_MULTIPLIER_DECREASE;
    static std::uint64_t POINT_VALUE_CONTRACT;
    static std::uint64_t POINT_VALUE_DODGE;
    static std::uint32_t DODGE_OBJECT_COOLDOWN;
    static double DODGE_RANGE;
    static double DODGE_SPEED_THRESHOLD;
    static std::uint32_t UPGRADE_LIST_SIZE;
    static std::uint32_t EFFECT_LENGTH_CARGO;
    static std::uint32_t EFFECT_LENGTH_HULL_DAMAGE;
    static std::uint32_t EFFECT_LENGTH_SMOKE;
    static double EFFECT_FADE_RATE;
    static std::uint32_t SHIELD_RECHARGE_RATE;
    static std::uint32_t DISABLED_LENGTH;
    static std::uint32_t MAX_POWER;
    static std::int32_t ITEM_RESTORE_HULL;
    static std::uint32_t ITEM_RESTORE_POWER;
    static std::uint32_t CONTRACT_RESTORE_POWER;
    static double ITEM_START_VELOCITY_MIN;
    static double ITEM_START_VELOCITY_MAX;
    static double ITEM_MAX_SPEED;
    static double SHIP_MANEUVER_ANGLE;
    static double SHIP_LANDING_RATE;
    static double POLICE_LIGHTS_ANGULAR_SPEED;
    static double POINT_DEFENSE_RANGE;
    static double ITEM_VACUUM_RANGE;
    static double ITEM_VACUUM_FORCE;
    static double MINIMUM_GEN_DISTANCE_BETWEEN_PLANETS;
    static double PLANETARY_SPACE_RANGE;
    static double GEN_PLAYER_START_CLEAN_DISTANCE;
    static std::uint32_t NOTORIETY_MAX;
    static std::uint32_t NOTORIETY_TIER_2;
    static std::uint32_t NOTORIETY_TIER_1;
    static std::uint32_t NOTORIETY_INCREASE_DAMAGE;
    static std::uint32_t NOTORIETY_INCREASE_SCAN;
    static std::uint32_t NOTORIETY_INCREASE_SCAN_INITIAL;
    static std::uint32_t WEAPON_SPREAD_DISTANCE;
    static std::uint32_t ACTIVE_POWER_DRAIN_RATE;
    static double CLOAK_OPACITY;
    static double WARP_ACCEL;
    static double WARP_DECEL;
    static double WARP_MAX_SPEED;
    static double DESPAWN_DISTANCE;
    static double NPC_PROCESS_RANGE;
    static std::uint32_t SPAWN_DISTANCE_MAX;
    static std::uint32_t SPAWN_DISTANCE_MIN;
    static std::uint32_t SHIP_SPAWN_RATE;
    static std::uint32_t ITEM_SPAWN_RATE;
    static std::uint32_t MAX_ATTEMPTS_SPAWN;
    static std::uint64_t DESIRED_SHIPS_MULTIPLIER;
    static std::uint64_t DESIRED_SHIPS_BASE;
    static std::uint64_t DESIRED_SHIPS_MAX;
    static std::uint64_t MIN_SCORE_MULTIPLIER_TIER_1;
    static std::uint64_t MIN_SCORE_MULTIPLIER_TIER_2;
    static std::uint32_t SCORE_MULTIPLIER_TIER_DENOMINATOR;
    static std::uint32_t SHIP_WEIGHT_CIVILIAN;
    static std::uint32_t SHIP_WEIGHT_PIRATE;
    static std::uint32_t SHIP_WEIGHT_POLICE_NOTORIETY_TIER_0;
    static std::uint32_t SHIP_WEIGHT_POLICE_NOTORIETY_TIER_1;
    static std::uint32_t SHIP_WEIGHT_POLICE_NOTORIETY_TIER_2;
    static std::uint32_t SHIP_WEIGHT_BOUNTY_HUNTER_NOTORIETY_TIER_1;
    static std::uint32_t SHIP_WEIGHT_BOUNTY_HUNTER_NOTORIETY_TIER_2;
    static double DESIRED_ITEMS_BASE;
    static double DESIRED_ITEMS_SCORE_MULTIPLIER_ADJUSTMENT;
    static double AI_RANGE;
    static double AI_FOLLOW_DISTANCE;
    static double AI_PATROL_DISTANCE;
    static double MISSILE_HOMING_RANGE;
    static double AI_ANGLE_MOD_IGNORE_DISTANCE;
    static double AI_ANGLE_MOD_DISTANCE;
    static double AI_WEIGHT_TARGET;
    static double AI_WEIGHT_AVOID_DEBRIS;
    static std::uint32_t AI_TIME_WITHOUT_MOVING_ALLOWED;
    static double AI_TIME_WITHOUT_MOVING_DISTANCE_THRESHOLD;
    static double TRACTOR_RANGE;
    static double TRACTOR_FORCE;
    static std::uint32_t MISSILE_HOMING_DELAY;
    static std::uint32_t SMOKE_DELAY;
    static std::uint32_t SMOKE_POSITION_MINIMUM;
    static std::uint32_t SMOKE_POSITION_MAXIMUM;
    static std::uint32_t SHOT_SMOKE_COUNT_SOLID;
    static std::uint32_t SHOT_SMOKE_COUNT_EXPLOSIVE;
    static std::uint32_t SHOT_SMOKE_COUNT_ENERGY;
    static std::uint32_t DEATH_HULL_COUNT_MINIMUM;
    static std::uint32_t DEATH_HULL_COUNT_MAXIMUM;
    static std::uint32_t DEATH_SMOKE_COUNT_MINIMUM;
    static std::uint32_t DEATH_SMOKE_COUNT_MAXIMUM;
    static std::uint32_t CLOUD_DELAY;
    static std::uint32_t CLOUD_CHANCE;
    static std::uint32_t AI_PROXIMITY_CHECK_PERIOD;
    static double LOW_HULL_THRESHOLD;
    static std::uint32_t DISABLED_SOUND_RATE;
    static std::uint32_t LOW_HULL_SOUND_RATE;
    static std::uint32_t CHASING_PLAYER_SOUND_RATE;
    static std::int32_t EXPLOSION_DAMAGE_SHIP;
    static double SHAKE_MAGNITUDE_DAMAGE;
    static std::int32_t SHAKE_LENGTH_DAMAGE;
    static double SHAKE_MAGNITUDE_WEAPON_SOLID;
    static std::int32_t SHAKE_LENGTH_WEAPON_SOLID;
    static double SHAKE_MAGNITUDE_WEAPON_EXPLOSIVE;
    static std::int32_t SHAKE_LENGTH_WEAPON_EXPLOSIVE;
    static double SHAKE_MAGNITUDE_WEAPON_ENERGY;
    static std::int32_t SHAKE_LENGTH_WEAPON_ENERGY;
    static double SCORE_MULTIPLIER_ACCEL;
    static double SCORE_MULTIPLIER_DECEL;
    static double SCORE_MULTIPLIER_MAX_SPEED;
    static std::uint64_t KILLS_TIER_1_CIVILIAN;
    static std::uint64_t KILLS_TIER_2_CIVILIAN;
    static std::uint64_t KILLS_TIER_3_CIVILIAN;
    static std::uint64_t KILLS_TIER_4_CIVILIAN;
    static std::uint64_t KILLS_TIER_1_PIRATE;
    static std::uint64_t KILLS_TIER_2_PIRATE;
    static std::uint64_t KILLS_TIER_3_PIRATE;
    static std::uint64_t KILLS_TIER_4_PIRATE;
    static std::uint64_t KILLS_TIER_1_POLICE;
    static std::uint64_t KILLS_TIER_2_POLICE;
    static std::uint64_t KILLS_TIER_3_POLICE;
    static std::uint64_t KILLS_TIER_4_POLICE;
    static std::uint64_t KILLS_TIER_1_BOUNTY_HUNTER;
    static std::uint64_t KILLS_TIER_2_BOUNTY_HUNTER;
    static std::uint64_t KILLS_TIER_3_BOUNTY_HUNTER;
    static std::uint64_t KILLS_TIER_4_BOUNTY_HUNTER;
    static std::uint64_t DODGES_TIER_1;
    static std::uint64_t DODGES_TIER_2;
    static std::uint64_t DODGES_TIER_3;
    static std::uint64_t DODGES_TIER_4;
    static std::uint64_t DODGES_TIER_5;
    static std::uint64_t DEFAULT_HIGH_SCORE_1;
    static std::uint64_t DEFAULT_HIGH_SCORE_2;
    static std::uint64_t DEFAULT_HIGH_SCORE_3;
    static std::uint64_t DEFAULT_HIGH_SCORE_4;
    static std::uint64_t DEFAULT_HIGH_SCORE_5;
    static std::uint64_t DEFAULT_HIGH_SCORE_6;
    static std::uint64_t DEFAULT_HIGH_SCORE_7;
    static std::uint64_t DEFAULT_HIGH_SCORE_8;
    static std::uint64_t DEFAULT_HIGH_SCORE_9;
    static std::uint64_t DEFAULT_HIGH_SCORE_10;
    static double SHIP_SHADOW_OFFSET;
    /// END SCRIPT-GENERATED CONSTANT DECLARATIONS
};

#endif
