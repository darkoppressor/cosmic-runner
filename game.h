/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef game_h
#define game_h

#include "ship.h"
#include "debris.h"
#include "effect.h"
#include "planet.h"
#include "shot.h"
#include "explosion.h"
#include "item.h"

#include <quadtree.h>
#include <rng.h>
#include <coords.h>
#include <sprite.h>

#include <vector>
#include <cstdint>
#include <string>

class Game{
private:

    static std::vector<Ship> ships;
    static std::vector<Debris> debris;
    static std::vector<Effect> effects;
    static std::vector<Planet> planets;
    static std::vector<Shot> shots;
    static std::vector<Explosion> explosions;
    static std::vector<Item> items;

    //contract target planet index
    static std::int32_t contract;

    static Sprite contract_sprite;
    static Sprite no_contract_sprite;

    //landed planet index
    static std::int32_t landed_planet;

    static std::uint64_t score;
    static std::uint64_t score_multiplier;

    static std::uint32_t power;

    static std::uint32_t notoriety;

    static bool player_tractored;
    static std::uint32_t tractoring_ship;
    static double tractor_angle;
    static Sprite tractor_sprite;

    //Simply used for information purposes
    static Vector player_acceleration;

    static std::vector<std::string> upgrade_list;

    static Quadtree<double,std::uint32_t> quadtree_debris;
    static Quadtree<double,std::uint32_t> quadtree_shots;
    static Quadtree<double,std::uint32_t> quadtree_ships;
    static Quadtree<double,std::uint32_t> quadtree_explosions;
    static Quadtree<double,std::uint32_t> quadtree_items;
    static Quadtree<double,std::uint32_t> quadtree_planets;

    static RNG rng;

    static std::uint32_t frame;
    static std::uint32_t ship_spawn_check;

    static Ship& get_player();

public:

    //pixels
    static double world_width;
    static double world_height;

    static void clear_world();
    static void generate_world();

    static std::uint32_t get_ship_count();
    static std::uint32_t get_debris_count();
    static std::uint32_t get_effect_count();
    static std::uint32_t get_planet_count();
    static const Ship& get_player_const();
    static const Debris& get_debris(std::uint32_t index);
    static const Shot& get_shot(std::uint32_t index);
    static const Ship& get_ship(std::uint32_t index);
    static const Planet& get_planet(std::uint32_t index);
    static const Explosion& get_explosion(std::uint32_t index);
    static const Item& get_item(std::uint32_t index);
    static std::uint64_t get_score();
    static std::uint64_t get_score_multiplier();
    static std::vector<std::string> get_upgrade_list();

    static RNG& get_rng();

    static void increase_score(std::uint64_t amount);
    static void increase_score_multiplier(std::uint64_t amount);
    static void decrease_score_multiplier(std::uint64_t amount);

    //Returns the index of the planet nearest to the ship with the passed index
    static std::uint32_t get_nearest_planet(std::uint32_t ship_index);
    //Returns the index of the planet nearest to the player's ship
    static std::uint32_t get_nearest_planet();

    static void assign_new_contract(std::uint32_t current_planet);
    static bool player_has_contract();
    static const Planet& get_contract_planet();
    static std::uint32_t get_contract_planet_index();
    static void complete_contract();
    static void cancel_contract();
    static void arrest_player();

    static void build_upgrade_list();

    static void commence_landing(uint32_t landing_planet_index);
    static void land(uint32_t landing_planet_index);
    static bool player_is_landing();
    static bool player_is_landed();
    static const Planet& get_landed_planet();

    static std::uint32_t get_power();
    static bool player_is_out_of_power();
    static void increase_power();

    static std::uint32_t get_notoriety();
    static bool notoriety_tier_1();
    static bool notoriety_tier_2();
    static void increase_notoriety();
    static void reset_notoriety();

    static bool is_player_tractored();
    static std::uint32_t get_tractoring_ship_index();
    static double get_tractor_angle();
    static const Sprite& get_tractor_sprite();
    static void clear_tractor();
    static void tractor_player(double angle,std::uint32_t ship_index);

    static Vector get_player_acceleration();
    static void set_player_acceleration(const Vector& acceleration);

    static void create_effect(std::string sprite,double scale,const Coords<double>& position,std::string sound,const Vector& velocity,double angle,
                              const Vector& angular_velocity,std::uint32_t seconds,bool line,const Coords<double>& end_point);

    static void player_thrust(std::string direction);
    static void player_brake(bool brake);
    static void player_toggle_weapons();
    static void player_add_upgrade(std::string name);
    static void player_remove_upgrade(std::string name);

    static void disable_ship(std::uint32_t index);

    static void kill_shot(std::uint32_t index);
    static void create_shot(std::uint32_t owner_index,std::string type,std::string faction,std::string firing_upgrade,const Coords<double>& position,double angle,std::int32_t damage_mod);

    static void create_explosion(std::string sprite,std::string sound,const Coords<double>& position,std::int32_t damage,std::string faction);

    static std::string get_random_item_type();
    static void kill_item(std::uint32_t index);
    static void create_item(const Coords<double>& position,const Vector& base_velocity);

    static void generate_ships();

    static void game_over();

    static void tick();
    static void ai();
    static void movement();
    static void events();
    static void animate();
    static void render();

    static void render_to_textures();

    static void update_background();
    static void render_background();

    static bool move_input_state(std::string direction);
};

#endif
