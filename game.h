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
#include "title.h"
#include "background.h"
#include "minimap.h"
#include "dodging.h"

#include <quadtree.h>
#include <rng.h>
#include <coords.h>
#include <sprite.h>
#include <math_vector.h>

#include <vector>
#include <cstdint>
#include <string>

class Game{
private:

    static Title title;

    static Background background_stars;
    static Background background_planetary;

    static double background_opacity_planetary;

    static Minimap minimap;
    static bool show_minimap;

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
    static Sprite contract_sprite_check;
    static Sprite no_contract_sprite;

    static Sprite police_lights_sprite;
    static Vector police_lights_angular_velocity;
    static double police_lights_angle;

    //landed planet index
    static std::int32_t landed_planet;

    static std::uint64_t score;
    static std::uint64_t score_multiplier;

    static std::uint32_t power;

    static std::uint32_t notoriety;

    static std::uint32_t kills;

    static bool player_tractored;
    static std::uint32_t tractoring_ship;
    static double tractor_angle;
    static Sprite tractor_sprite;

    //Simply used for information purposes
    static Vector player_acceleration;

    static Dodging dodging;

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
    static std::uint32_t item_spawn_check;

    static std::uint32_t sound_cooldown_disabled;
    static std::uint32_t sound_cooldown_low_hull;

    static Ship& get_player();

    static void dodge_check();

public:

    //pixels
    static double world_width;
    static double world_height;

    static void clear_world();
    static void generate_world();

    static std::uint32_t get_ship_count();
    static std::uint32_t get_item_count();
    static std::uint32_t get_debris_count();
    static std::uint32_t get_effect_count();
    static std::uint32_t get_planet_count();
    static std::uint32_t get_explosion_count();
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

    static Title& get_title();

    static RNG& get_rng();

    static void toggle_minimap();

    static void increase_score(std::uint64_t amount, bool add_effect = true);
    static void increase_score_multiplier(std::uint64_t amount);
    static void decrease_score_multiplier(std::uint64_t amount);

    //Returns the index of the planet nearest to the ship with the passed index
    static std::uint32_t get_nearest_planet(std::uint32_t ship_index);
    //Returns the index of the planet nearest to the player's ship
    static std::uint32_t get_nearest_planet();

    static bool is_player_in_deep_space();

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

    static const Sprite& get_police_lights_sprite();
    static double get_police_lights_angle();

    static std::uint32_t get_power();
    static bool player_is_out_of_power();
    static void increase_power_item();
    static void increase_power_contract();
    static void decrease_power();
    static void use_power(std::uint32_t amount);

    static std::uint32_t get_notoriety();
    static bool notoriety_tier_1();
    static bool notoriety_tier_2();
    static void increase_notoriety(std::uint32_t amount);
    static void reset_notoriety();

    static std::uint32_t get_kills();
    static void add_kill();

    static bool is_player_tractored();
    static std::uint32_t get_tractoring_ship_index();
    static double get_tractor_angle();
    static const Sprite& get_tractor_sprite();
    static void clear_tractor();
    static void tractor_player(double angle,std::uint32_t ship_index);

    static Vector get_player_acceleration();
    static void set_player_acceleration(const Vector& acceleration);

    static void create_effect(std::string sprite,bool fade,double scale,const Coords<double>& position,std::string sound,const Vector& velocity,double angle,
                              const Vector& angular_velocity,std::uint32_t seconds,bool line,const Coords<double>& end_point,std::string color="white",std::string text="");

    static void player_thrust(std::string direction);
    static void player_brake(bool brake);
    static void player_toggle_weapons();
    static void player_use_active();
    static void player_add_upgrade(std::string name);
    static void player_remove_upgrade(std::string name);

    static void disable_ship(std::uint32_t index);

    static void kill_shot(std::uint32_t index);
    static void create_shot(std::uint32_t owner_index,std::string type,std::string faction,std::string firing_upgrade,const Coords<double>& position,double angle,std::int32_t damage_mod);

    static void create_explosion(std::string sprite,std::string sound,const Coords<double>& position,std::int32_t damage,std::string faction,bool scan=false,bool emp=false);

    static std::string get_random_item_type();
    static void kill_item(std::uint32_t index);
    static void create_item(const Coords<double>& position,const Vector& base_velocity);

    //Returns true if the passed rectangle collides with any debris
    static bool collides_with_game_world(const Collision_Rect<double>& box);

    //Returns a random point within an area on a randomly selected side of the player
    //This function can fail, in which case it returns coordinates of -1.0,-1.0
    static Coords<double> get_spawn_point(double width,double height);

    static void generate_ships();
    static void generate_items();

    static void game_over();

    static void handle_repeating_sounds();

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

    static bool is_object_over_planet(const std::vector<Coords<double>>& vertices,const Planet& planet);
};

#endif
