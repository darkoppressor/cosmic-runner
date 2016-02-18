/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game_constants.h"

#include <game_constants_loader.h>
#include <engine_strings.h>
#include <engine_data.h>

using namespace std;

uint32_t Game_Constants::TITLE_SHIP_SPAWN_RATE=0;
uint32_t Game_Constants::TITLE_MAX_SHIPS=0;
uint32_t Game_Constants::TITLE_SHIP_MINIMUM_HEIGHT=0;

uint32_t Game_Constants::TITLE_MAX_SUN_SHIMMERS=0;
uint32_t Game_Constants::TITLE_SUN_SHIMMER_SPAWN_RATE=0;
uint32_t Game_Constants::TITLE_SUN_SHIMMER_SPAWN_CHANCE=0;
uint32_t Game_Constants::TITLE_SUN_SHIMMER_Y_RIGHT_MOD_MIN=0;
uint32_t Game_Constants::TITLE_SUN_SHIMMER_Y_RIGHT_MOD_MAX=0;
uint32_t Game_Constants::TITLE_SUN_SHIMMER_SPEED_MIN=0;
uint32_t Game_Constants::TITLE_SUN_SHIMMER_SPEED_MAX=0;
uint32_t Game_Constants::TITLE_SUN_SHIMMER_RADIUS_MIN=0;
uint32_t Game_Constants::TITLE_SUN_SHIMMER_RADIUS_MAX=0;

uint32_t Game_Constants::MAX_BACKGROUND_STAR_SIZE=0;
uint32_t Game_Constants::BASE_BACKGROUND_STARS=0;
uint32_t Game_Constants::MAX_BACKGROUND_DEBRIS_SIZE=0;
uint32_t Game_Constants::BASE_BACKGROUND_DEBRIS=0;
double Game_Constants::BACKGROUND_FADE_RATE=0.0;

uint32_t Game_Constants::COLLISION_CHANCE_DEBRIS=0;

uint64_t Game_Constants::SCORE_MULTIPLIER_INCREASE=0;
uint64_t Game_Constants::SCORE_MULTIPLIER_DECREASE=0;
uint64_t Game_Constants::POINT_VALUE_CONTRACT=0;

uint32_t Game_Constants::UPGRADE_LIST_SIZE=0;

uint32_t Game_Constants::EFFECT_LENGTH_CARGO=0;
uint32_t Game_Constants::EFFECT_LENGTH_HULL_DAMAGE=0;
double Game_Constants::EFFECT_FADE_RATE=0.0;

uint32_t Game_Constants::SHIELD_RECHARGE_RATE=0;
uint32_t Game_Constants::DISABLED_LENGTH=0;
uint32_t Game_Constants::MAX_POWER=0;

int32_t Game_Constants::ITEM_RESTORE_HULL=0;
uint32_t Game_Constants::ITEM_RESTORE_POWER=0;

double Game_Constants::ITEM_START_VELOCITY_MIN=0.0;
double Game_Constants::ITEM_START_VELOCITY_MAX=0.0;
double Game_Constants::ITEM_MAX_SPEED=0.0;

double Game_Constants::SHIP_MANEUVER_ANGLE=0.0;

double Game_Constants::SHIP_LANDING_RATE=0.0;

double Game_Constants::POLICE_LIGHTS_ANGULAR_SPEED=0.0;

double Game_Constants::POINT_DEFENSE_RANGE=0.0;

double Game_Constants::ITEM_VACUUM_RANGE=0.0;
double Game_Constants::ITEM_VACUUM_FORCE=0.0;

double Game_Constants::STAR_RADIUS=0.0;
double Game_Constants::STAR_DAMAGE_RANGE=0.0;
uint32_t Game_Constants::STAR_DAMAGE_RATE=0;
int32_t Game_Constants::STAR_DAMAGE=0;
double Game_Constants::MINIMUM_GEN_DISTANCE_FROM_STAR=0.0;
double Game_Constants::MINIMUM_GEN_DISTANCE_BETWEEN_PLANETS=0.0;

double Game_Constants::PLANETARY_SPACE_RANGE=0.0;

double Game_Constants::GEN_PLAYER_START_CLEAN_DISTANCE=0.0;

uint32_t Game_Constants::NOTORIETY_MAX=0;
uint32_t Game_Constants::NOTORIETY_TIER_2=0;
uint32_t Game_Constants::NOTORIETY_TIER_1=0;
uint32_t Game_Constants::NOTORIETY_INCREASE_DAMAGE=0;
uint32_t Game_Constants::NOTORIETY_INCREASE_SCAN=0;

uint32_t Game_Constants::WEAPON_SPREAD_DISTANCE=0;

uint32_t Game_Constants::ACTIVE_POWER_DRAIN_RATE=0;
double Game_Constants::CLOAK_OPACITY=0.0;

double Game_Constants::DESPAWN_DISTANCE=0.0;
double Game_Constants::NPC_PROCESS_RANGE=0.0;
uint32_t Game_Constants::SPAWN_DISTANCE_MAX=0;
uint32_t Game_Constants::SPAWN_DISTANCE_MIN=0;
uint32_t Game_Constants::SHIP_SPAWN_RATE=0;
uint32_t Game_Constants::ITEM_SPAWN_RATE=0;

uint32_t Game_Constants::MAX_ATTEMPTS_SPAWN=0;

uint64_t Game_Constants::DESIRED_SHIPS_MULTIPLIER=0;
uint64_t Game_Constants::DESIRED_SHIPS_MAX=0;
uint64_t Game_Constants::MIN_SCORE_MULTIPLIER_TIER_1=0;
uint64_t Game_Constants::MIN_SCORE_MULTIPLIER_TIER_2=0;
uint32_t Game_Constants::SCORE_MULTIPLIER_TIER_DENOMINATOR=0;
uint32_t Game_Constants::SHIP_WEIGHT_CIVILIAN=0;
uint32_t Game_Constants::SHIP_WEIGHT_PIRATE=0;
uint32_t Game_Constants::SHIP_WEIGHT_POLICE_NOTORIETY_TIER_0=0;
uint32_t Game_Constants::SHIP_WEIGHT_POLICE_NOTORIETY_TIER_1=0;
uint32_t Game_Constants::SHIP_WEIGHT_POLICE_NOTORIETY_TIER_2=0;
uint32_t Game_Constants::SHIP_WEIGHT_BOUNTY_HUNTER_NOTORIETY_TIER_1=0;
uint32_t Game_Constants::SHIP_WEIGHT_BOUNTY_HUNTER_NOTORIETY_TIER_2=0;

double Game_Constants::DESIRED_ITEMS_BASE=0.0;
double Game_Constants::DESIRED_ITEMS_SCORE_MULTIPLIER_ADJUSTMENT=0.0;

double Game_Constants::AI_RANGE=0.0;
double Game_Constants::AI_FOLLOW_DISTANCE=0.0;
double Game_Constants::AI_PATROL_DISTANCE=0.0;
double Game_Constants::MISSILE_HOMING_RANGE=0.0;

double Game_Constants::TRACTOR_RANGE=0.0;
double Game_Constants::TRACTOR_FORCE=0.0;

uint32_t Game_Constants::MISSILE_HOMING_DELAY=0;

uint32_t Game_Constants::AI_PROXIMITY_CHECK_PERIOD=0;

int32_t Game_Constants::EXPLOSION_DAMAGE_SHIP=0;

void Game_Constants_Loader::set_game_constant(string name,string value){
    if(name=="zoom_rate"){
        Engine_Data::ZOOM_RATE=Strings::string_to_double(value);
    }
    else if(name=="zoom_min"){
        Engine_Data::ZOOM_MIN=Strings::string_to_double(value);
    }
    else if(name=="zoom_max"){
        Engine_Data::ZOOM_MAX=Strings::string_to_double(value);
    }

    else if(name=="title_ship_spawn_rate"){
        Game_Constants::TITLE_SHIP_SPAWN_RATE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="title_max_ships"){
        Game_Constants::TITLE_MAX_SHIPS=Strings::string_to_unsigned_long(value);
    }
    else if(name=="title_ship_minimum_height"){
        Game_Constants::TITLE_SHIP_MINIMUM_HEIGHT=Strings::string_to_unsigned_long(value);
    }

    else if(name=="title_max_sun_shimmers"){
        Game_Constants::TITLE_MAX_SUN_SHIMMERS=Strings::string_to_unsigned_long(value);
    }
    else if(name=="title_sun_shimmer_spawn_rate"){
        Game_Constants::TITLE_SUN_SHIMMER_SPAWN_RATE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="title_sun_shimmer_spawn_chance"){
        Game_Constants::TITLE_SUN_SHIMMER_SPAWN_CHANCE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="title_sun_shimmer_y_right_mod_min"){
        Game_Constants::TITLE_SUN_SHIMMER_Y_RIGHT_MOD_MIN=Strings::string_to_unsigned_long(value);
    }
    else if(name=="title_sun_shimmer_y_right_mod_max"){
        Game_Constants::TITLE_SUN_SHIMMER_Y_RIGHT_MOD_MAX=Strings::string_to_unsigned_long(value);
    }
    else if(name=="title_sun_shimmer_speed_min"){
        Game_Constants::TITLE_SUN_SHIMMER_SPEED_MIN=Strings::string_to_unsigned_long(value);
    }
    else if(name=="title_sun_shimmer_speed_max"){
        Game_Constants::TITLE_SUN_SHIMMER_SPEED_MAX=Strings::string_to_unsigned_long(value);
    }
    else if(name=="title_sun_shimmer_radius_min"){
        Game_Constants::TITLE_SUN_SHIMMER_RADIUS_MIN=Strings::string_to_unsigned_long(value);
    }
    else if(name=="title_sun_shimmer_radius_max"){
        Game_Constants::TITLE_SUN_SHIMMER_RADIUS_MAX=Strings::string_to_unsigned_long(value);
    }

    else if(name=="max_background_star_size"){
        Game_Constants::MAX_BACKGROUND_STAR_SIZE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="base_background_stars"){
        Game_Constants::BASE_BACKGROUND_STARS=Strings::string_to_unsigned_long(value);
    }
    else if(name=="max_background_debris_size"){
        Game_Constants::MAX_BACKGROUND_DEBRIS_SIZE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="base_background_debris"){
        Game_Constants::BASE_BACKGROUND_DEBRIS=Strings::string_to_unsigned_long(value);
    }
    else if(name=="background_fade_rate"){
        Game_Constants::BACKGROUND_FADE_RATE=Strings::string_to_double(value);
    }

    else if(name=="collision_chance_debris"){
        Game_Constants::COLLISION_CHANCE_DEBRIS=Strings::string_to_unsigned_long(value);
    }

    else if(name=="score_multiplier_increase"){
        Game_Constants::SCORE_MULTIPLIER_INCREASE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="score_multiplier_decrease"){
        Game_Constants::SCORE_MULTIPLIER_DECREASE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="point_value_contract"){
        Game_Constants::POINT_VALUE_CONTRACT=Strings::string_to_unsigned_long(value);
    }

    else if(name=="upgrade_list_size"){
        Game_Constants::UPGRADE_LIST_SIZE=Strings::string_to_unsigned_long(value);
    }

    else if(name=="effect_length_cargo"){
        Game_Constants::EFFECT_LENGTH_CARGO=Strings::string_to_unsigned_long(value);
    }
    else if(name=="effect_length_hull_damage"){
        Game_Constants::EFFECT_LENGTH_HULL_DAMAGE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="effect_fade_rate"){
        Game_Constants::EFFECT_FADE_RATE=Strings::string_to_double(value);
    }

    else if(name=="shield_recharge_rate"){
        Game_Constants::SHIELD_RECHARGE_RATE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="disabled_length"){
        Game_Constants::DISABLED_LENGTH=Strings::string_to_unsigned_long(value);
    }
    else if(name=="max_power"){
        Game_Constants::MAX_POWER=Strings::string_to_unsigned_long(value);
    }

    else if(name=="item_restore_hull"){
        Game_Constants::ITEM_RESTORE_HULL=Strings::string_to_long(value);
    }
    else if(name=="item_restore_power"){
        Game_Constants::ITEM_RESTORE_POWER=Strings::string_to_unsigned_long(value);
    }

    else if(name=="item_start_velocity_min"){
        Game_Constants::ITEM_START_VELOCITY_MIN=Strings::string_to_double(value);
    }
    else if(name=="item_start_velocity_max"){
        Game_Constants::ITEM_START_VELOCITY_MAX=Strings::string_to_double(value);
    }
    else if(name=="item_max_speed"){
        Game_Constants::ITEM_MAX_SPEED=Strings::string_to_double(value);
    }

    else if(name=="ship_maneuver_angle"){
        Game_Constants::SHIP_MANEUVER_ANGLE=Strings::string_to_double(value);
    }

    else if(name=="ship_landing_rate"){
        Game_Constants::SHIP_LANDING_RATE=Strings::string_to_double(value);
    }

    else if(name=="police_lights_angular_speed"){
        Game_Constants::POLICE_LIGHTS_ANGULAR_SPEED=Strings::string_to_double(value);
    }

    else if(name=="point_defense_range"){
        Game_Constants::POINT_DEFENSE_RANGE=Strings::string_to_double(value);
    }

    else if(name=="item_vacuum_range"){
        Game_Constants::ITEM_VACUUM_RANGE=Strings::string_to_double(value);
    }
    else if(name=="item_vacuum_force"){
        Game_Constants::ITEM_VACUUM_FORCE=Strings::string_to_double(value);
    }

    else if(name=="star_radius"){
        Game_Constants::STAR_RADIUS=Strings::string_to_double(value);
    }
    else if(name=="star_damage_range"){
        Game_Constants::STAR_DAMAGE_RANGE=Strings::string_to_double(value);
    }
    else if(name=="star_damage_rate"){
        Game_Constants::STAR_DAMAGE_RATE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="star_damage"){
        Game_Constants::STAR_DAMAGE=Strings::string_to_long(value);
    }
    else if(name=="minimum_gen_distance_from_star"){
        Game_Constants::MINIMUM_GEN_DISTANCE_FROM_STAR=Strings::string_to_double(value);
    }
    else if(name=="minimum_gen_distance_between_planets"){
        Game_Constants::MINIMUM_GEN_DISTANCE_BETWEEN_PLANETS=Strings::string_to_double(value);
    }

    else if(name=="planetary_space_range"){
        Game_Constants::PLANETARY_SPACE_RANGE=Strings::string_to_double(value);
    }

    else if(name=="gen_player_start_clean_distance"){
        Game_Constants::GEN_PLAYER_START_CLEAN_DISTANCE=Strings::string_to_double(value);
    }

    else if(name=="notoriety_max"){
        Game_Constants::NOTORIETY_MAX=Strings::string_to_unsigned_long(value);
    }
    else if(name=="notoriety_tier_2"){
        Game_Constants::NOTORIETY_TIER_2=Strings::string_to_unsigned_long(value);
    }
    else if(name=="notoriety_tier_1"){
        Game_Constants::NOTORIETY_TIER_1=Strings::string_to_unsigned_long(value);
    }
    else if(name=="notoriety_increase_damage"){
        Game_Constants::NOTORIETY_INCREASE_DAMAGE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="notoriety_increase_scan"){
        Game_Constants::NOTORIETY_INCREASE_SCAN=Strings::string_to_unsigned_long(value);
    }

    else if(name=="weapon_spread_distance"){
        Game_Constants::WEAPON_SPREAD_DISTANCE=Strings::string_to_unsigned_long(value);
    }

    else if(name=="active_power_drain_rate"){
        Game_Constants::ACTIVE_POWER_DRAIN_RATE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="cloak_opacity"){
        Game_Constants::CLOAK_OPACITY=Strings::string_to_double(value);
    }

    else if(name=="despawn_distance"){
        Game_Constants::DESPAWN_DISTANCE=Strings::string_to_double(value);
    }
    else if(name=="npc_process_range"){
        Game_Constants::NPC_PROCESS_RANGE=Strings::string_to_double(value);
    }
    else if(name=="spawn_distance_max"){
        Game_Constants::SPAWN_DISTANCE_MAX=Strings::string_to_unsigned_long(value);
    }
    else if(name=="spawn_distance_min"){
        Game_Constants::SPAWN_DISTANCE_MIN=Strings::string_to_unsigned_long(value);
    }
    else if(name=="ship_spawn_rate"){
        Game_Constants::SHIP_SPAWN_RATE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="item_spawn_rate"){
        Game_Constants::ITEM_SPAWN_RATE=Strings::string_to_unsigned_long(value);
    }

    else if(name=="max_attempts_spawn"){
        Game_Constants::MAX_ATTEMPTS_SPAWN=Strings::string_to_unsigned_long(value);
    }

    else if(name=="desired_ships_multiplier"){
        Game_Constants::DESIRED_SHIPS_MULTIPLIER=Strings::string_to_unsigned_long(value);
    }
    else if(name=="desired_ships_max"){
        Game_Constants::DESIRED_SHIPS_MAX=Strings::string_to_unsigned_long(value);
    }
    else if(name=="min_score_multiplier_tier_1"){
        Game_Constants::MIN_SCORE_MULTIPLIER_TIER_1=Strings::string_to_unsigned_long(value);
    }
    else if(name=="min_score_multiplier_tier_2"){
        Game_Constants::MIN_SCORE_MULTIPLIER_TIER_2=Strings::string_to_unsigned_long(value);
    }
    else if(name=="score_multiplier_tier_denominator"){
        Game_Constants::SCORE_MULTIPLIER_TIER_DENOMINATOR=Strings::string_to_unsigned_long(value);
    }
    else if(name=="ship_weight_civilian"){
        Game_Constants::SHIP_WEIGHT_CIVILIAN=Strings::string_to_unsigned_long(value);
    }
    else if(name=="ship_weight_pirate"){
        Game_Constants::SHIP_WEIGHT_PIRATE=Strings::string_to_unsigned_long(value);
    }
    else if(name=="ship_weight_police_notoriety_tier_0"){
        Game_Constants::SHIP_WEIGHT_POLICE_NOTORIETY_TIER_0=Strings::string_to_unsigned_long(value);
    }
    else if(name=="ship_weight_police_notoriety_tier_1"){
        Game_Constants::SHIP_WEIGHT_POLICE_NOTORIETY_TIER_1=Strings::string_to_unsigned_long(value);
    }
    else if(name=="ship_weight_police_notoriety_tier_2"){
        Game_Constants::SHIP_WEIGHT_POLICE_NOTORIETY_TIER_2=Strings::string_to_unsigned_long(value);
    }
    else if(name=="ship_weight_bounty_hunter_notoriety_tier_1"){
        Game_Constants::SHIP_WEIGHT_BOUNTY_HUNTER_NOTORIETY_TIER_1=Strings::string_to_unsigned_long(value);
    }
    else if(name=="ship_weight_bounty_hunter_notoriety_tier_2"){
        Game_Constants::SHIP_WEIGHT_BOUNTY_HUNTER_NOTORIETY_TIER_2=Strings::string_to_unsigned_long(value);
    }

    else if(name=="desired_items_base"){
        Game_Constants::DESIRED_ITEMS_BASE=Strings::string_to_double(value);
    }
    else if(name=="desired_items_score_multiplier_adjustment"){
        Game_Constants::DESIRED_ITEMS_SCORE_MULTIPLIER_ADJUSTMENT=Strings::string_to_double(value);
    }

    else if(name=="ai_range"){
        Game_Constants::AI_RANGE=Strings::string_to_double(value);
    }
    else if(name=="ai_follow_distance"){
        Game_Constants::AI_FOLLOW_DISTANCE=Strings::string_to_double(value);
    }
    else if(name=="ai_patrol_distance"){
        Game_Constants::AI_PATROL_DISTANCE=Strings::string_to_double(value);
    }
    else if(name=="missile_homing_range"){
        Game_Constants::MISSILE_HOMING_RANGE=Strings::string_to_double(value);
    }

    else if(name=="tractor_range"){
        Game_Constants::TRACTOR_RANGE=Strings::string_to_double(value);
    }
    else if(name=="tractor_force"){
        Game_Constants::TRACTOR_FORCE=Strings::string_to_double(value);
    }

    else if(name=="missile_homing_delay"){
        Game_Constants::MISSILE_HOMING_DELAY=Strings::string_to_unsigned_long(value);
    }

    else if(name=="ai_proximity_check_period"){
        Game_Constants::AI_PROXIMITY_CHECK_PERIOD=Strings::string_to_unsigned_long(value);
    }

    else if(name=="explosion_damage_ship"){
        Game_Constants::EXPLOSION_DAMAGE_SHIP=Strings::string_to_long(value);
    }
}
