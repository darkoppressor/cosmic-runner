/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game_constants.h"

#include <game_constants_loader.h>
#include <engine_strings.h>
#include <engine_data.h>

using namespace std;

// BEGIN SCRIPT-GENERATED CONSTANT INITIALIZATIONS
uint32_t Game_Constants::TITLE_SHIP_SPAWN_RATE = 0;
uint32_t Game_Constants::TITLE_MAX_SHIPS = 0;
uint32_t Game_Constants::TITLE_BIRD_CHANCE = 0;
uint32_t Game_Constants::TITLE_CLOUD_CHANCE = 0;
uint32_t Game_Constants::TITLE_SHIP_MINIMUM_HEIGHT = 0;
uint32_t Game_Constants::TITLE_MAX_TREES_MINIMUM = 0;
uint32_t Game_Constants::TITLE_MAX_TREES_MAXIMUM = 0;
uint32_t Game_Constants::TITLE_TREE_MAXIMUM_HEIGHT = 0;
uint32_t Game_Constants::TITLE_TREE_ANIMATION_SPEED_MINIMUM = 0;
uint32_t Game_Constants::TITLE_TREE_ANIMATION_SPEED_MAXIMUM = 0;
uint32_t Game_Constants::TITLE_MAX_SUN_SHIMMERS = 0;
uint32_t Game_Constants::TITLE_SUN_SHIMMER_SPAWN_RATE = 0;
uint32_t Game_Constants::TITLE_SUN_SHIMMER_SPAWN_CHANCE = 0;
uint32_t Game_Constants::TITLE_SUN_SHIMMER_Y_RIGHT_MOD_MIN = 0;
uint32_t Game_Constants::TITLE_SUN_SHIMMER_Y_RIGHT_MOD_MAX = 0;
uint32_t Game_Constants::TITLE_SUN_SHIMMER_SPEED_MIN = 0;
uint32_t Game_Constants::TITLE_SUN_SHIMMER_SPEED_MAX = 0;
uint32_t Game_Constants::TITLE_SUN_SHIMMER_RADIUS_MIN = 0;
uint32_t Game_Constants::TITLE_SUN_SHIMMER_RADIUS_MAX = 0;
uint32_t Game_Constants::MAX_BACKGROUND_STAR_SIZE = 0;
uint32_t Game_Constants::BASE_BACKGROUND_STARS = 0;
uint32_t Game_Constants::MAX_BACKGROUND_DEBRIS_SIZE = 0;
uint32_t Game_Constants::BASE_BACKGROUND_DEBRIS = 0;
double Game_Constants::BACKGROUND_FADE_RATE = 0.0;
double Game_Constants::HUD_SPACING = 0.0;
double Game_Constants::HUD_MAX_BAR_WIDTH = 0.0;
double Game_Constants::HUD_BAR_HEIGHT = 0.0;
double Game_Constants::HUD_BAR_OFFSET_X = 0.0;
double Game_Constants::HUD_BAR_OFFSET_Y = 0.0;
double Game_Constants::MINIMAP_BORDER_THICKNESS = 0.0;
double Game_Constants::MINIMAP_SHIP_SCALE = 0.0;
double Game_Constants::MINIMAP_ARROW_MAGNITUDE = 0.0;
double Game_Constants::MINIMAP_ARROW_ANGLE_MOD = 0.0;
uint32_t Game_Constants::MINIMAP_SIZE = 0;
int32_t Game_Constants::MINIMAP_LINE_THICKNESS = 0;
uint32_t Game_Constants::COLLISION_CHANCE_DEBRIS = 0;
uint64_t Game_Constants::SCORE_MULTIPLIER_INCREASE = 0;
uint64_t Game_Constants::SCORE_MULTIPLIER_DECREASE = 0;
uint64_t Game_Constants::POINT_VALUE_CONTRACT = 0;
uint64_t Game_Constants::POINT_VALUE_DODGE = 0;
uint32_t Game_Constants::DODGE_OBJECT_COOLDOWN = 0;
double Game_Constants::DODGE_RANGE = 0.0;
double Game_Constants::DODGE_SPEED_THRESHOLD = 0.0;
uint32_t Game_Constants::UPGRADE_LIST_SIZE = 0;
uint32_t Game_Constants::EFFECT_LENGTH_CARGO = 0;
uint32_t Game_Constants::EFFECT_LENGTH_HULL_DAMAGE = 0;
uint32_t Game_Constants::EFFECT_LENGTH_SMOKE = 0;
double Game_Constants::EFFECT_FADE_RATE = 0.0;
uint32_t Game_Constants::SHIELD_RECHARGE_RATE = 0;
uint32_t Game_Constants::DISABLED_LENGTH = 0;
uint32_t Game_Constants::MAX_POWER = 0;
int32_t Game_Constants::ITEM_RESTORE_HULL = 0;
uint32_t Game_Constants::ITEM_RESTORE_POWER = 0;
int32_t Game_Constants::CONTRACT_RESTORE_HULL = 0;
uint32_t Game_Constants::CONTRACT_RESTORE_POWER = 0;
double Game_Constants::ITEM_START_VELOCITY_MIN = 0.0;
double Game_Constants::ITEM_START_VELOCITY_MAX = 0.0;
double Game_Constants::ITEM_MAX_SPEED = 0.0;
double Game_Constants::SHIP_MANEUVER_ANGLE = 0.0;
double Game_Constants::SHIP_LANDING_RATE = 0.0;
double Game_Constants::POLICE_LIGHTS_ANGULAR_SPEED = 0.0;
double Game_Constants::POINT_DEFENSE_RANGE = 0.0;
double Game_Constants::ITEM_VACUUM_RANGE = 0.0;
double Game_Constants::ITEM_VACUUM_FORCE = 0.0;
double Game_Constants::MINIMUM_GEN_DISTANCE_BETWEEN_PLANETS = 0.0;
double Game_Constants::PLANETARY_SPACE_RANGE = 0.0;
double Game_Constants::GEN_PLAYER_START_CLEAN_DISTANCE = 0.0;
uint32_t Game_Constants::NOTORIETY_MAX = 0;
uint32_t Game_Constants::NOTORIETY_TIER_2 = 0;
uint32_t Game_Constants::NOTORIETY_TIER_1 = 0;
uint32_t Game_Constants::NOTORIETY_INCREASE_DAMAGE = 0;
uint32_t Game_Constants::NOTORIETY_INCREASE_SCAN = 0;
uint32_t Game_Constants::NOTORIETY_INCREASE_SCAN_INITIAL = 0;
uint32_t Game_Constants::WEAPON_SPREAD_DISTANCE = 0;
uint32_t Game_Constants::ACTIVE_POWER_DRAIN_RATE = 0;
double Game_Constants::CLOAK_OPACITY = 0.0;
double Game_Constants::WARP_ACCEL = 0.0;
double Game_Constants::WARP_DECEL = 0.0;
double Game_Constants::WARP_MAX_SPEED = 0.0;
double Game_Constants::DESPAWN_DISTANCE = 0.0;
double Game_Constants::NPC_PROCESS_RANGE = 0.0;
uint32_t Game_Constants::SPAWN_DISTANCE_MAX = 0;
uint32_t Game_Constants::SPAWN_DISTANCE_MIN = 0;
uint32_t Game_Constants::SHIP_SPAWN_RATE = 0;
uint32_t Game_Constants::ITEM_SPAWN_RATE = 0;
uint32_t Game_Constants::MAX_ATTEMPTS_SPAWN = 0;
uint64_t Game_Constants::DESIRED_SHIPS_MULTIPLIER = 0;
uint64_t Game_Constants::DESIRED_SHIPS_BASE = 0;
uint64_t Game_Constants::DESIRED_SHIPS_MAX = 0;
uint64_t Game_Constants::MIN_SCORE_MULTIPLIER_TIER_1 = 0;
uint64_t Game_Constants::MIN_SCORE_MULTIPLIER_TIER_2 = 0;
uint32_t Game_Constants::SCORE_MULTIPLIER_TIER_DENOMINATOR = 0;
uint32_t Game_Constants::SHIP_WEIGHT_CIVILIAN = 0;
uint32_t Game_Constants::SHIP_WEIGHT_PIRATE = 0;
uint32_t Game_Constants::SHIP_WEIGHT_POLICE_NOTORIETY_TIER_0 = 0;
uint32_t Game_Constants::SHIP_WEIGHT_POLICE_NOTORIETY_TIER_1 = 0;
uint32_t Game_Constants::SHIP_WEIGHT_POLICE_NOTORIETY_TIER_2 = 0;
uint32_t Game_Constants::SHIP_WEIGHT_BOUNTY_HUNTER_NOTORIETY_TIER_1 = 0;
uint32_t Game_Constants::SHIP_WEIGHT_BOUNTY_HUNTER_NOTORIETY_TIER_2 = 0;
double Game_Constants::DESIRED_ITEMS_BASE = 0.0;
double Game_Constants::DESIRED_ITEMS_SCORE_MULTIPLIER_ADJUSTMENT = 0.0;
double Game_Constants::AI_RANGE = 0.0;
double Game_Constants::AI_FOLLOW_DISTANCE = 0.0;
double Game_Constants::AI_PATROL_DISTANCE = 0.0;
double Game_Constants::MISSILE_HOMING_RANGE = 0.0;
double Game_Constants::AI_ANGLE_MOD_IGNORE_DISTANCE = 0.0;
double Game_Constants::AI_ANGLE_MOD_DISTANCE = 0.0;
double Game_Constants::AI_WEIGHT_TARGET = 0.0;
double Game_Constants::AI_WEIGHT_AVOID_DEBRIS = 0.0;
uint32_t Game_Constants::AI_TIME_WITHOUT_MOVING_ALLOWED = 0;
double Game_Constants::AI_TIME_WITHOUT_MOVING_DISTANCE_THRESHOLD = 0.0;
double Game_Constants::TRACTOR_RANGE = 0.0;
double Game_Constants::TRACTOR_FORCE = 0.0;
uint32_t Game_Constants::MISSILE_HOMING_DELAY = 0;
uint32_t Game_Constants::SMOKE_DELAY = 0;
uint32_t Game_Constants::SMOKE_POSITION_MINIMUM = 0;
uint32_t Game_Constants::SMOKE_POSITION_MAXIMUM = 0;
uint32_t Game_Constants::SHOT_SMOKE_COUNT_SOLID = 0;
uint32_t Game_Constants::SHOT_SMOKE_COUNT_EXPLOSIVE = 0;
uint32_t Game_Constants::SHOT_SMOKE_COUNT_ENERGY = 0;
uint32_t Game_Constants::DEATH_HULL_COUNT_MINIMUM = 0;
uint32_t Game_Constants::DEATH_HULL_COUNT_MAXIMUM = 0;
uint32_t Game_Constants::DEATH_SMOKE_COUNT_MINIMUM = 0;
uint32_t Game_Constants::DEATH_SMOKE_COUNT_MAXIMUM = 0;
uint32_t Game_Constants::CLOUD_DELAY = 0;
uint32_t Game_Constants::CLOUD_CHANCE = 0;
uint32_t Game_Constants::AI_PROXIMITY_CHECK_PERIOD = 0;
double Game_Constants::LOW_HULL_THRESHOLD = 0.0;
uint32_t Game_Constants::DISABLED_SOUND_RATE = 0;
uint32_t Game_Constants::LOW_HULL_SOUND_RATE = 0;
uint32_t Game_Constants::CHASING_PLAYER_SOUND_RATE = 0;
int32_t Game_Constants::EXPLOSION_DAMAGE_SHIP = 0;
double Game_Constants::SHAKE_MAGNITUDE_DAMAGE = 0.0;
int32_t Game_Constants::SHAKE_LENGTH_DAMAGE = 0;
double Game_Constants::SHAKE_MAGNITUDE_WEAPON_SOLID = 0.0;
int32_t Game_Constants::SHAKE_LENGTH_WEAPON_SOLID = 0;
double Game_Constants::SHAKE_MAGNITUDE_WEAPON_EXPLOSIVE = 0.0;
int32_t Game_Constants::SHAKE_LENGTH_WEAPON_EXPLOSIVE = 0;
double Game_Constants::SHAKE_MAGNITUDE_WEAPON_ENERGY = 0.0;
int32_t Game_Constants::SHAKE_LENGTH_WEAPON_ENERGY = 0;
double Game_Constants::SCORE_MULTIPLIER_ACCEL = 0.0;
double Game_Constants::SCORE_MULTIPLIER_DECEL = 0.0;
double Game_Constants::SCORE_MULTIPLIER_MAX_SPEED = 0.0;
uint64_t Game_Constants::KILLS_TIER_1_CIVILIAN = 0;
uint64_t Game_Constants::KILLS_TIER_2_CIVILIAN = 0;
uint64_t Game_Constants::KILLS_TIER_3_CIVILIAN = 0;
uint64_t Game_Constants::KILLS_TIER_4_CIVILIAN = 0;
uint64_t Game_Constants::KILLS_TIER_1_PIRATE = 0;
uint64_t Game_Constants::KILLS_TIER_2_PIRATE = 0;
uint64_t Game_Constants::KILLS_TIER_3_PIRATE = 0;
uint64_t Game_Constants::KILLS_TIER_4_PIRATE = 0;
uint64_t Game_Constants::KILLS_TIER_1_POLICE = 0;
uint64_t Game_Constants::KILLS_TIER_2_POLICE = 0;
uint64_t Game_Constants::KILLS_TIER_3_POLICE = 0;
uint64_t Game_Constants::KILLS_TIER_4_POLICE = 0;
uint64_t Game_Constants::KILLS_TIER_1_BOUNTY_HUNTER = 0;
uint64_t Game_Constants::KILLS_TIER_2_BOUNTY_HUNTER = 0;
uint64_t Game_Constants::KILLS_TIER_3_BOUNTY_HUNTER = 0;
uint64_t Game_Constants::KILLS_TIER_4_BOUNTY_HUNTER = 0;
uint64_t Game_Constants::DODGES_TIER_1 = 0;
uint64_t Game_Constants::DODGES_TIER_2 = 0;
uint64_t Game_Constants::DODGES_TIER_3 = 0;
uint64_t Game_Constants::DODGES_TIER_4 = 0;
uint64_t Game_Constants::DODGES_TIER_5 = 0;
uint64_t Game_Constants::DEFAULT_HIGH_SCORE_1 = 0;
uint64_t Game_Constants::DEFAULT_HIGH_SCORE_2 = 0;
uint64_t Game_Constants::DEFAULT_HIGH_SCORE_3 = 0;
uint64_t Game_Constants::DEFAULT_HIGH_SCORE_4 = 0;
uint64_t Game_Constants::DEFAULT_HIGH_SCORE_5 = 0;
uint64_t Game_Constants::DEFAULT_HIGH_SCORE_6 = 0;
uint64_t Game_Constants::DEFAULT_HIGH_SCORE_7 = 0;
uint64_t Game_Constants::DEFAULT_HIGH_SCORE_8 = 0;
uint64_t Game_Constants::DEFAULT_HIGH_SCORE_9 = 0;
uint64_t Game_Constants::DEFAULT_HIGH_SCORE_10 = 0;
double Game_Constants::SHADOW_OFFSET = 0.0;
double Game_Constants::SHADOW_OFFSET_DEBRIS = 0.0;

// END SCRIPT-GENERATED CONSTANT INITIALIZATIONS
void Game_Constants_Loader::set_game_constant (string name, string value) {
    if (name == "zoom_rate") {
        Engine_Data::ZOOM_RATE = Strings::string_to_double(value);
    } else if (name == "zoom_min") {
        Engine_Data::ZOOM_MIN = Strings::string_to_double(value);
    } else if (name == "zoom_max") {
        Engine_Data::ZOOM_MAX = Strings::string_to_double(value);
    }
    // BEGIN SCRIPT-GENERATED CONSTANT SETUP
    else if (name == "title_ship_spawn_rate") {
        Game_Constants::TITLE_SHIP_SPAWN_RATE = Strings::string_to_unsigned_long(value);
    } else if (name == "title_max_ships") {
        Game_Constants::TITLE_MAX_SHIPS = Strings::string_to_unsigned_long(value);
    } else if (name == "title_bird_chance") {
        Game_Constants::TITLE_BIRD_CHANCE = Strings::string_to_unsigned_long(value);
    } else if (name == "title_cloud_chance") {
        Game_Constants::TITLE_CLOUD_CHANCE = Strings::string_to_unsigned_long(value);
    } else if (name == "title_ship_minimum_height") {
        Game_Constants::TITLE_SHIP_MINIMUM_HEIGHT = Strings::string_to_unsigned_long(value);
    } else if (name == "title_max_trees_minimum") {
        Game_Constants::TITLE_MAX_TREES_MINIMUM = Strings::string_to_unsigned_long(value);
    } else if (name == "title_max_trees_maximum") {
        Game_Constants::TITLE_MAX_TREES_MAXIMUM = Strings::string_to_unsigned_long(value);
    } else if (name == "title_tree_maximum_height") {
        Game_Constants::TITLE_TREE_MAXIMUM_HEIGHT = Strings::string_to_unsigned_long(value);
    } else if (name == "title_tree_animation_speed_minimum") {
        Game_Constants::TITLE_TREE_ANIMATION_SPEED_MINIMUM = Strings::string_to_unsigned_long(value);
    } else if (name == "title_tree_animation_speed_maximum") {
        Game_Constants::TITLE_TREE_ANIMATION_SPEED_MAXIMUM = Strings::string_to_unsigned_long(value);
    } else if (name == "title_max_sun_shimmers") {
        Game_Constants::TITLE_MAX_SUN_SHIMMERS = Strings::string_to_unsigned_long(value);
    } else if (name == "title_sun_shimmer_spawn_rate") {
        Game_Constants::TITLE_SUN_SHIMMER_SPAWN_RATE = Strings::string_to_unsigned_long(value);
    } else if (name == "title_sun_shimmer_spawn_chance") {
        Game_Constants::TITLE_SUN_SHIMMER_SPAWN_CHANCE = Strings::string_to_unsigned_long(value);
    } else if (name == "title_sun_shimmer_y_right_mod_min") {
        Game_Constants::TITLE_SUN_SHIMMER_Y_RIGHT_MOD_MIN = Strings::string_to_unsigned_long(value);
    } else if (name == "title_sun_shimmer_y_right_mod_max") {
        Game_Constants::TITLE_SUN_SHIMMER_Y_RIGHT_MOD_MAX = Strings::string_to_unsigned_long(value);
    } else if (name == "title_sun_shimmer_speed_min") {
        Game_Constants::TITLE_SUN_SHIMMER_SPEED_MIN = Strings::string_to_unsigned_long(value);
    } else if (name == "title_sun_shimmer_speed_max") {
        Game_Constants::TITLE_SUN_SHIMMER_SPEED_MAX = Strings::string_to_unsigned_long(value);
    } else if (name == "title_sun_shimmer_radius_min") {
        Game_Constants::TITLE_SUN_SHIMMER_RADIUS_MIN = Strings::string_to_unsigned_long(value);
    } else if (name == "title_sun_shimmer_radius_max") {
        Game_Constants::TITLE_SUN_SHIMMER_RADIUS_MAX = Strings::string_to_unsigned_long(value);
    } else if (name == "max_background_star_size") {
        Game_Constants::MAX_BACKGROUND_STAR_SIZE = Strings::string_to_unsigned_long(value);
    } else if (name == "base_background_stars") {
        Game_Constants::BASE_BACKGROUND_STARS = Strings::string_to_unsigned_long(value);
    } else if (name == "max_background_debris_size") {
        Game_Constants::MAX_BACKGROUND_DEBRIS_SIZE = Strings::string_to_unsigned_long(value);
    } else if (name == "base_background_debris") {
        Game_Constants::BASE_BACKGROUND_DEBRIS = Strings::string_to_unsigned_long(value);
    } else if (name == "background_fade_rate") {
        Game_Constants::BACKGROUND_FADE_RATE = Strings::string_to_double(value);
    } else if (name == "hud_spacing") {
        Game_Constants::HUD_SPACING = Strings::string_to_double(value);
    } else if (name == "hud_max_bar_width") {
        Game_Constants::HUD_MAX_BAR_WIDTH = Strings::string_to_double(value);
    } else if (name == "hud_bar_height") {
        Game_Constants::HUD_BAR_HEIGHT = Strings::string_to_double(value);
    } else if (name == "hud_bar_offset_x") {
        Game_Constants::HUD_BAR_OFFSET_X = Strings::string_to_double(value);
    } else if (name == "hud_bar_offset_y") {
        Game_Constants::HUD_BAR_OFFSET_Y = Strings::string_to_double(value);
    } else if (name == "minimap_border_thickness") {
        Game_Constants::MINIMAP_BORDER_THICKNESS = Strings::string_to_double(value);
    } else if (name == "minimap_ship_scale") {
        Game_Constants::MINIMAP_SHIP_SCALE = Strings::string_to_double(value);
    } else if (name == "minimap_arrow_magnitude") {
        Game_Constants::MINIMAP_ARROW_MAGNITUDE = Strings::string_to_double(value);
    } else if (name == "minimap_arrow_angle_mod") {
        Game_Constants::MINIMAP_ARROW_ANGLE_MOD = Strings::string_to_double(value);
    } else if (name == "minimap_size") {
        Game_Constants::MINIMAP_SIZE = Strings::string_to_unsigned_long(value);
    } else if (name == "minimap_line_thickness") {
        Game_Constants::MINIMAP_LINE_THICKNESS = Strings::string_to_long(value);
    } else if (name == "collision_chance_debris") {
        Game_Constants::COLLISION_CHANCE_DEBRIS = Strings::string_to_unsigned_long(value);
    } else if (name == "score_multiplier_increase") {
        Game_Constants::SCORE_MULTIPLIER_INCREASE = Strings::string_to_unsigned_long(value);
    } else if (name == "score_multiplier_decrease") {
        Game_Constants::SCORE_MULTIPLIER_DECREASE = Strings::string_to_unsigned_long(value);
    } else if (name == "point_value_contract") {
        Game_Constants::POINT_VALUE_CONTRACT = Strings::string_to_unsigned_long(value);
    } else if (name == "point_value_dodge") {
        Game_Constants::POINT_VALUE_DODGE = Strings::string_to_unsigned_long(value);
    } else if (name == "dodge_object_cooldown") {
        Game_Constants::DODGE_OBJECT_COOLDOWN = Strings::string_to_unsigned_long(value);
    } else if (name == "dodge_range") {
        Game_Constants::DODGE_RANGE = Strings::string_to_double(value);
    } else if (name == "dodge_speed_threshold") {
        Game_Constants::DODGE_SPEED_THRESHOLD = Strings::string_to_double(value);
    } else if (name == "upgrade_list_size") {
        Game_Constants::UPGRADE_LIST_SIZE = Strings::string_to_unsigned_long(value);
    } else if (name == "effect_length_cargo") {
        Game_Constants::EFFECT_LENGTH_CARGO = Strings::string_to_unsigned_long(value);
    } else if (name == "effect_length_hull_damage") {
        Game_Constants::EFFECT_LENGTH_HULL_DAMAGE = Strings::string_to_unsigned_long(value);
    } else if (name == "effect_length_smoke") {
        Game_Constants::EFFECT_LENGTH_SMOKE = Strings::string_to_unsigned_long(value);
    } else if (name == "effect_fade_rate") {
        Game_Constants::EFFECT_FADE_RATE = Strings::string_to_double(value);
    } else if (name == "shield_recharge_rate") {
        Game_Constants::SHIELD_RECHARGE_RATE = Strings::string_to_unsigned_long(value);
    } else if (name == "disabled_length") {
        Game_Constants::DISABLED_LENGTH = Strings::string_to_unsigned_long(value);
    } else if (name == "max_power") {
        Game_Constants::MAX_POWER = Strings::string_to_unsigned_long(value);
    } else if (name == "item_restore_hull") {
        Game_Constants::ITEM_RESTORE_HULL = Strings::string_to_long(value);
    } else if (name == "item_restore_power") {
        Game_Constants::ITEM_RESTORE_POWER = Strings::string_to_unsigned_long(value);
    } else if (name == "contract_restore_hull") {
        Game_Constants::CONTRACT_RESTORE_HULL = Strings::string_to_long(value);
    } else if (name == "contract_restore_power") {
        Game_Constants::CONTRACT_RESTORE_POWER = Strings::string_to_unsigned_long(value);
    } else if (name == "item_start_velocity_min") {
        Game_Constants::ITEM_START_VELOCITY_MIN = Strings::string_to_double(value);
    } else if (name == "item_start_velocity_max") {
        Game_Constants::ITEM_START_VELOCITY_MAX = Strings::string_to_double(value);
    } else if (name == "item_max_speed") {
        Game_Constants::ITEM_MAX_SPEED = Strings::string_to_double(value);
    } else if (name == "ship_maneuver_angle") {
        Game_Constants::SHIP_MANEUVER_ANGLE = Strings::string_to_double(value);
    } else if (name == "ship_landing_rate") {
        Game_Constants::SHIP_LANDING_RATE = Strings::string_to_double(value);
    } else if (name == "police_lights_angular_speed") {
        Game_Constants::POLICE_LIGHTS_ANGULAR_SPEED = Strings::string_to_double(value);
    } else if (name == "point_defense_range") {
        Game_Constants::POINT_DEFENSE_RANGE = Strings::string_to_double(value);
    } else if (name == "item_vacuum_range") {
        Game_Constants::ITEM_VACUUM_RANGE = Strings::string_to_double(value);
    } else if (name == "item_vacuum_force") {
        Game_Constants::ITEM_VACUUM_FORCE = Strings::string_to_double(value);
    } else if (name == "minimum_gen_distance_between_planets") {
        Game_Constants::MINIMUM_GEN_DISTANCE_BETWEEN_PLANETS = Strings::string_to_double(value);
    } else if (name == "planetary_space_range") {
        Game_Constants::PLANETARY_SPACE_RANGE = Strings::string_to_double(value);
    } else if (name == "gen_player_start_clean_distance") {
        Game_Constants::GEN_PLAYER_START_CLEAN_DISTANCE = Strings::string_to_double(value);
    } else if (name == "notoriety_max") {
        Game_Constants::NOTORIETY_MAX = Strings::string_to_unsigned_long(value);
    } else if (name == "notoriety_tier_2") {
        Game_Constants::NOTORIETY_TIER_2 = Strings::string_to_unsigned_long(value);
    } else if (name == "notoriety_tier_1") {
        Game_Constants::NOTORIETY_TIER_1 = Strings::string_to_unsigned_long(value);
    } else if (name == "notoriety_increase_damage") {
        Game_Constants::NOTORIETY_INCREASE_DAMAGE = Strings::string_to_unsigned_long(value);
    } else if (name == "notoriety_increase_scan") {
        Game_Constants::NOTORIETY_INCREASE_SCAN = Strings::string_to_unsigned_long(value);
    } else if (name == "notoriety_increase_scan_initial") {
        Game_Constants::NOTORIETY_INCREASE_SCAN_INITIAL = Strings::string_to_unsigned_long(value);
    } else if (name == "weapon_spread_distance") {
        Game_Constants::WEAPON_SPREAD_DISTANCE = Strings::string_to_unsigned_long(value);
    } else if (name == "active_power_drain_rate") {
        Game_Constants::ACTIVE_POWER_DRAIN_RATE = Strings::string_to_unsigned_long(value);
    } else if (name == "cloak_opacity") {
        Game_Constants::CLOAK_OPACITY = Strings::string_to_double(value);
    } else if (name == "warp_accel") {
        Game_Constants::WARP_ACCEL = Strings::string_to_double(value);
    } else if (name == "warp_decel") {
        Game_Constants::WARP_DECEL = Strings::string_to_double(value);
    } else if (name == "warp_max_speed") {
        Game_Constants::WARP_MAX_SPEED = Strings::string_to_double(value);
    } else if (name == "despawn_distance") {
        Game_Constants::DESPAWN_DISTANCE = Strings::string_to_double(value);
    } else if (name == "npc_process_range") {
        Game_Constants::NPC_PROCESS_RANGE = Strings::string_to_double(value);
    } else if (name == "spawn_distance_max") {
        Game_Constants::SPAWN_DISTANCE_MAX = Strings::string_to_unsigned_long(value);
    } else if (name == "spawn_distance_min") {
        Game_Constants::SPAWN_DISTANCE_MIN = Strings::string_to_unsigned_long(value);
    } else if (name == "ship_spawn_rate") {
        Game_Constants::SHIP_SPAWN_RATE = Strings::string_to_unsigned_long(value);
    } else if (name == "item_spawn_rate") {
        Game_Constants::ITEM_SPAWN_RATE = Strings::string_to_unsigned_long(value);
    } else if (name == "max_attempts_spawn") {
        Game_Constants::MAX_ATTEMPTS_SPAWN = Strings::string_to_unsigned_long(value);
    } else if (name == "desired_ships_multiplier") {
        Game_Constants::DESIRED_SHIPS_MULTIPLIER = Strings::string_to_unsigned_long(value);
    } else if (name == "desired_ships_base") {
        Game_Constants::DESIRED_SHIPS_BASE = Strings::string_to_unsigned_long(value);
    } else if (name == "desired_ships_max") {
        Game_Constants::DESIRED_SHIPS_MAX = Strings::string_to_unsigned_long(value);
    } else if (name == "min_score_multiplier_tier_1") {
        Game_Constants::MIN_SCORE_MULTIPLIER_TIER_1 = Strings::string_to_unsigned_long(value);
    } else if (name == "min_score_multiplier_tier_2") {
        Game_Constants::MIN_SCORE_MULTIPLIER_TIER_2 = Strings::string_to_unsigned_long(value);
    } else if (name == "score_multiplier_tier_denominator") {
        Game_Constants::SCORE_MULTIPLIER_TIER_DENOMINATOR = Strings::string_to_unsigned_long(value);
    } else if (name == "ship_weight_civilian") {
        Game_Constants::SHIP_WEIGHT_CIVILIAN = Strings::string_to_unsigned_long(value);
    } else if (name == "ship_weight_pirate") {
        Game_Constants::SHIP_WEIGHT_PIRATE = Strings::string_to_unsigned_long(value);
    } else if (name == "ship_weight_police_notoriety_tier_0") {
        Game_Constants::SHIP_WEIGHT_POLICE_NOTORIETY_TIER_0 = Strings::string_to_unsigned_long(value);
    } else if (name == "ship_weight_police_notoriety_tier_1") {
        Game_Constants::SHIP_WEIGHT_POLICE_NOTORIETY_TIER_1 = Strings::string_to_unsigned_long(value);
    } else if (name == "ship_weight_police_notoriety_tier_2") {
        Game_Constants::SHIP_WEIGHT_POLICE_NOTORIETY_TIER_2 = Strings::string_to_unsigned_long(value);
    } else if (name == "ship_weight_bounty_hunter_notoriety_tier_1") {
        Game_Constants::SHIP_WEIGHT_BOUNTY_HUNTER_NOTORIETY_TIER_1 = Strings::string_to_unsigned_long(value);
    } else if (name == "ship_weight_bounty_hunter_notoriety_tier_2") {
        Game_Constants::SHIP_WEIGHT_BOUNTY_HUNTER_NOTORIETY_TIER_2 = Strings::string_to_unsigned_long(value);
    } else if (name == "desired_items_base") {
        Game_Constants::DESIRED_ITEMS_BASE = Strings::string_to_double(value);
    } else if (name == "desired_items_score_multiplier_adjustment") {
        Game_Constants::DESIRED_ITEMS_SCORE_MULTIPLIER_ADJUSTMENT = Strings::string_to_double(value);
    } else if (name == "ai_range") {
        Game_Constants::AI_RANGE = Strings::string_to_double(value);
    } else if (name == "ai_follow_distance") {
        Game_Constants::AI_FOLLOW_DISTANCE = Strings::string_to_double(value);
    } else if (name == "ai_patrol_distance") {
        Game_Constants::AI_PATROL_DISTANCE = Strings::string_to_double(value);
    } else if (name == "missile_homing_range") {
        Game_Constants::MISSILE_HOMING_RANGE = Strings::string_to_double(value);
    } else if (name == "ai_angle_mod_ignore_distance") {
        Game_Constants::AI_ANGLE_MOD_IGNORE_DISTANCE = Strings::string_to_double(value);
    } else if (name == "ai_angle_mod_distance") {
        Game_Constants::AI_ANGLE_MOD_DISTANCE = Strings::string_to_double(value);
    } else if (name == "ai_weight_target") {
        Game_Constants::AI_WEIGHT_TARGET = Strings::string_to_double(value);
    } else if (name == "ai_weight_avoid_debris") {
        Game_Constants::AI_WEIGHT_AVOID_DEBRIS = Strings::string_to_double(value);
    } else if (name == "ai_time_without_moving_allowed") {
        Game_Constants::AI_TIME_WITHOUT_MOVING_ALLOWED = Strings::string_to_unsigned_long(value);
    } else if (name == "ai_time_without_moving_distance_threshold") {
        Game_Constants::AI_TIME_WITHOUT_MOVING_DISTANCE_THRESHOLD = Strings::string_to_double(value);
    } else if (name == "tractor_range") {
        Game_Constants::TRACTOR_RANGE = Strings::string_to_double(value);
    } else if (name == "tractor_force") {
        Game_Constants::TRACTOR_FORCE = Strings::string_to_double(value);
    } else if (name == "missile_homing_delay") {
        Game_Constants::MISSILE_HOMING_DELAY = Strings::string_to_unsigned_long(value);
    } else if (name == "smoke_delay") {
        Game_Constants::SMOKE_DELAY = Strings::string_to_unsigned_long(value);
    } else if (name == "smoke_position_minimum") {
        Game_Constants::SMOKE_POSITION_MINIMUM = Strings::string_to_unsigned_long(value);
    } else if (name == "smoke_position_maximum") {
        Game_Constants::SMOKE_POSITION_MAXIMUM = Strings::string_to_unsigned_long(value);
    } else if (name == "shot_smoke_count_solid") {
        Game_Constants::SHOT_SMOKE_COUNT_SOLID = Strings::string_to_unsigned_long(value);
    } else if (name == "shot_smoke_count_explosive") {
        Game_Constants::SHOT_SMOKE_COUNT_EXPLOSIVE = Strings::string_to_unsigned_long(value);
    } else if (name == "shot_smoke_count_energy") {
        Game_Constants::SHOT_SMOKE_COUNT_ENERGY = Strings::string_to_unsigned_long(value);
    } else if (name == "death_hull_count_minimum") {
        Game_Constants::DEATH_HULL_COUNT_MINIMUM = Strings::string_to_unsigned_long(value);
    } else if (name == "death_hull_count_maximum") {
        Game_Constants::DEATH_HULL_COUNT_MAXIMUM = Strings::string_to_unsigned_long(value);
    } else if (name == "death_smoke_count_minimum") {
        Game_Constants::DEATH_SMOKE_COUNT_MINIMUM = Strings::string_to_unsigned_long(value);
    } else if (name == "death_smoke_count_maximum") {
        Game_Constants::DEATH_SMOKE_COUNT_MAXIMUM = Strings::string_to_unsigned_long(value);
    } else if (name == "cloud_delay") {
        Game_Constants::CLOUD_DELAY = Strings::string_to_unsigned_long(value);
    } else if (name == "cloud_chance") {
        Game_Constants::CLOUD_CHANCE = Strings::string_to_unsigned_long(value);
    } else if (name == "ai_proximity_check_period") {
        Game_Constants::AI_PROXIMITY_CHECK_PERIOD = Strings::string_to_unsigned_long(value);
    } else if (name == "low_hull_threshold") {
        Game_Constants::LOW_HULL_THRESHOLD = Strings::string_to_double(value);
    } else if (name == "disabled_sound_rate") {
        Game_Constants::DISABLED_SOUND_RATE = Strings::string_to_unsigned_long(value);
    } else if (name == "low_hull_sound_rate") {
        Game_Constants::LOW_HULL_SOUND_RATE = Strings::string_to_unsigned_long(value);
    } else if (name == "chasing_player_sound_rate") {
        Game_Constants::CHASING_PLAYER_SOUND_RATE = Strings::string_to_unsigned_long(value);
    } else if (name == "explosion_damage_ship") {
        Game_Constants::EXPLOSION_DAMAGE_SHIP = Strings::string_to_long(value);
    } else if (name == "shake_magnitude_damage") {
        Game_Constants::SHAKE_MAGNITUDE_DAMAGE = Strings::string_to_double(value);
    } else if (name == "shake_length_damage") {
        Game_Constants::SHAKE_LENGTH_DAMAGE = Strings::string_to_long(value);
    } else if (name == "shake_magnitude_weapon_solid") {
        Game_Constants::SHAKE_MAGNITUDE_WEAPON_SOLID = Strings::string_to_double(value);
    } else if (name == "shake_length_weapon_solid") {
        Game_Constants::SHAKE_LENGTH_WEAPON_SOLID = Strings::string_to_long(value);
    } else if (name == "shake_magnitude_weapon_explosive") {
        Game_Constants::SHAKE_MAGNITUDE_WEAPON_EXPLOSIVE = Strings::string_to_double(value);
    } else if (name == "shake_length_weapon_explosive") {
        Game_Constants::SHAKE_LENGTH_WEAPON_EXPLOSIVE = Strings::string_to_long(value);
    } else if (name == "shake_magnitude_weapon_energy") {
        Game_Constants::SHAKE_MAGNITUDE_WEAPON_ENERGY = Strings::string_to_double(value);
    } else if (name == "shake_length_weapon_energy") {
        Game_Constants::SHAKE_LENGTH_WEAPON_ENERGY = Strings::string_to_long(value);
    } else if (name == "score_multiplier_accel") {
        Game_Constants::SCORE_MULTIPLIER_ACCEL = Strings::string_to_double(value);
    } else if (name == "score_multiplier_decel") {
        Game_Constants::SCORE_MULTIPLIER_DECEL = Strings::string_to_double(value);
    } else if (name == "score_multiplier_max_speed") {
        Game_Constants::SCORE_MULTIPLIER_MAX_SPEED = Strings::string_to_double(value);
    } else if (name == "kills_tier_1_civilian") {
        Game_Constants::KILLS_TIER_1_CIVILIAN = Strings::string_to_unsigned_long(value);
    } else if (name == "kills_tier_2_civilian") {
        Game_Constants::KILLS_TIER_2_CIVILIAN = Strings::string_to_unsigned_long(value);
    } else if (name == "kills_tier_3_civilian") {
        Game_Constants::KILLS_TIER_3_CIVILIAN = Strings::string_to_unsigned_long(value);
    } else if (name == "kills_tier_4_civilian") {
        Game_Constants::KILLS_TIER_4_CIVILIAN = Strings::string_to_unsigned_long(value);
    } else if (name == "kills_tier_1_pirate") {
        Game_Constants::KILLS_TIER_1_PIRATE = Strings::string_to_unsigned_long(value);
    } else if (name == "kills_tier_2_pirate") {
        Game_Constants::KILLS_TIER_2_PIRATE = Strings::string_to_unsigned_long(value);
    } else if (name == "kills_tier_3_pirate") {
        Game_Constants::KILLS_TIER_3_PIRATE = Strings::string_to_unsigned_long(value);
    } else if (name == "kills_tier_4_pirate") {
        Game_Constants::KILLS_TIER_4_PIRATE = Strings::string_to_unsigned_long(value);
    } else if (name == "kills_tier_1_police") {
        Game_Constants::KILLS_TIER_1_POLICE = Strings::string_to_unsigned_long(value);
    } else if (name == "kills_tier_2_police") {
        Game_Constants::KILLS_TIER_2_POLICE = Strings::string_to_unsigned_long(value);
    } else if (name == "kills_tier_3_police") {
        Game_Constants::KILLS_TIER_3_POLICE = Strings::string_to_unsigned_long(value);
    } else if (name == "kills_tier_4_police") {
        Game_Constants::KILLS_TIER_4_POLICE = Strings::string_to_unsigned_long(value);
    } else if (name == "kills_tier_1_bounty_hunter") {
        Game_Constants::KILLS_TIER_1_BOUNTY_HUNTER = Strings::string_to_unsigned_long(value);
    } else if (name == "kills_tier_2_bounty_hunter") {
        Game_Constants::KILLS_TIER_2_BOUNTY_HUNTER = Strings::string_to_unsigned_long(value);
    } else if (name == "kills_tier_3_bounty_hunter") {
        Game_Constants::KILLS_TIER_3_BOUNTY_HUNTER = Strings::string_to_unsigned_long(value);
    } else if (name == "kills_tier_4_bounty_hunter") {
        Game_Constants::KILLS_TIER_4_BOUNTY_HUNTER = Strings::string_to_unsigned_long(value);
    } else if (name == "dodges_tier_1") {
        Game_Constants::DODGES_TIER_1 = Strings::string_to_unsigned_long(value);
    } else if (name == "dodges_tier_2") {
        Game_Constants::DODGES_TIER_2 = Strings::string_to_unsigned_long(value);
    } else if (name == "dodges_tier_3") {
        Game_Constants::DODGES_TIER_3 = Strings::string_to_unsigned_long(value);
    } else if (name == "dodges_tier_4") {
        Game_Constants::DODGES_TIER_4 = Strings::string_to_unsigned_long(value);
    } else if (name == "dodges_tier_5") {
        Game_Constants::DODGES_TIER_5 = Strings::string_to_unsigned_long(value);
    } else if (name == "default_high_score_1") {
        Game_Constants::DEFAULT_HIGH_SCORE_1 = Strings::string_to_unsigned_long(value);
    } else if (name == "default_high_score_2") {
        Game_Constants::DEFAULT_HIGH_SCORE_2 = Strings::string_to_unsigned_long(value);
    } else if (name == "default_high_score_3") {
        Game_Constants::DEFAULT_HIGH_SCORE_3 = Strings::string_to_unsigned_long(value);
    } else if (name == "default_high_score_4") {
        Game_Constants::DEFAULT_HIGH_SCORE_4 = Strings::string_to_unsigned_long(value);
    } else if (name == "default_high_score_5") {
        Game_Constants::DEFAULT_HIGH_SCORE_5 = Strings::string_to_unsigned_long(value);
    } else if (name == "default_high_score_6") {
        Game_Constants::DEFAULT_HIGH_SCORE_6 = Strings::string_to_unsigned_long(value);
    } else if (name == "default_high_score_7") {
        Game_Constants::DEFAULT_HIGH_SCORE_7 = Strings::string_to_unsigned_long(value);
    } else if (name == "default_high_score_8") {
        Game_Constants::DEFAULT_HIGH_SCORE_8 = Strings::string_to_unsigned_long(value);
    } else if (name == "default_high_score_9") {
        Game_Constants::DEFAULT_HIGH_SCORE_9 = Strings::string_to_unsigned_long(value);
    } else if (name == "default_high_score_10") {
        Game_Constants::DEFAULT_HIGH_SCORE_10 = Strings::string_to_unsigned_long(value);
    } else if (name == "shadow_offset") {
        Game_Constants::SHADOW_OFFSET = Strings::string_to_double(value);
    } else if (name == "shadow_offset_debris") {
        Game_Constants::SHADOW_OFFSET_DEBRIS = Strings::string_to_double(value);
    }

    // END SCRIPT-GENERATED CONSTANT SETUP
}
