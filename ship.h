/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef ship_h
#define ship_h

#include "ship_type.h"
#include "item_type.h"

#include <collision.h>
#include <math_vector.h>
#include <coords.h>
#include <sprite.h>
#include <quadtree.h>
#include <rng.h>

#include <string>
#include <cstdint>
#include <vector>

class Ship{
private:

    Collision_Rect<double> box;

    Vector velocity;
    Vector force;

    double angle;

    std::string type;

    std::int32_t hull;
    std::int32_t shields;

    bool thrusting;
    bool braking;

    bool landing;
    double landing_scale;
    std::uint32_t landing_planet_index;

    std::uint32_t shield_recharge;

    std::vector<std::string> upgrades;
    bool weapons_enabled;
    std::uint32_t weapon_cooldown;

    //Indices of explosions that have hit this ship
    std::vector<std::uint32_t> explosions;

    bool laser_has_target;
    Coords<double> laser_endpoint;

    std::uint32_t disabled_cooldown;

    Coords<double> ai_target;
    //An extra target held in reserve for police ships
    //They patrol between this point and ai_target
    Coords<double> ai_target_next;
    std::uint32_t ai_proximity_target;
    bool ai_has_proximity_target;
    bool ai_proximity_target_flee;

    Sprite sprite;

public:

    Ship(std::string new_type,const Coords<double>& position,double new_angle);

    Ship_Type* get_ship_type() const;

    Collision_Rect<double> get_box() const;
    Vector get_velocity() const;
    double get_angle() const;
    std::int32_t get_hull() const;
    std::int32_t get_shields() const;

    std::uint32_t get_shield_recharge_rate() const;

    bool has_upgrade(std::string name) const;
    bool has_weapon() const;
    std::string get_weapon_name() const;
    void remove_weapon();
    bool has_active() const;
    std::string get_active_name() const;
    void remove_active();
    bool are_weapons_enabled() const;
    void toggle_weapons();
    void add_upgrade(std::string name);
    void remove_upgrade(std::string name);

    bool has_laser() const;
    void calculate_laser_target(const Quadtree<double,std::uint32_t>& quadtree_ships,std::uint32_t own_index);
    void render_laser(bool is_player);

    bool is_disabled(bool is_player) const;
    void disable();

    bool can_use_item(Item_Type* item_type) const;
    void use_item(Item_Type* item_type);

    void apply_tractor(double force_angle);

    double get_thrust_accel() const;
    double get_thrust_decel() const;
    double get_max_speed() const;
    std::int32_t get_hull_max() const;
    std::int32_t get_shields_max() const;
    std::string get_faction() const;

    Collision_Rect<double> get_collision_box() const;

    bool is_alive() const;

    double get_distance_to_player() const;
    double get_distance_to_proximity_target() const;

    bool was_damaged_by_explosion(std::uint32_t index) const;
    void damaged_by_explosion(std::uint32_t index);
    void notify_of_explosion_death(std::uint32_t index);

    void clear_proximity_target();
    void notify_of_ship_death(std::uint32_t index);

    void take_damage(bool is_player,std::int32_t damage,std::string damage_type,const Coords<double>& location,std::string damage_faction,RNG& rng);

    void set_thrust_angle(std::string direction);
    void set_braking(bool new_braking);

    //Returns the distance between the two passed angles as a number within [0,180], casted to a double
    double get_distance_between_angles(double angle_a,double angle_b) const;

    void thrust(std::uint32_t frame);
    void brake(std::uint32_t frame);

    void stop();

    void commence_landing(std::uint32_t new_landing_planet_index);
    bool is_landing() const;
    void land(bool is_player);

    void regenerate_shields(bool is_player);
    void cooldown(const Quadtree<double,std::uint32_t>& quadtree_ships,RNG& rng,std::uint32_t own_index);

    bool faction_is_valid(std::string faction,bool weapon_check) const;

    std::int32_t get_nearest_valid_target_ship(const Quadtree<double,std::uint32_t>& quadtree_ships,std::uint32_t own_index,const Collision_Rect<double>& box_targeting,bool weapon_check);

    //Returns true if the weapon found a target and fired upon it
    bool fire_weapon(const Quadtree<double,std::uint32_t>& quadtree_ships,RNG& rng,std::uint32_t own_index);

    void ai_select_target(std::uint32_t own_index,RNG& rng);
    void ai_check_for_proximity_target(const Quadtree<double,std::uint32_t>& quadtree_ships,std::uint32_t own_index);
    bool ai_proximity_check_allowed(std::uint32_t frame,std::uint32_t own_index) const;
    void ai(const Quadtree<double,std::uint32_t>& quadtree_ships,const Quadtree<double,std::uint32_t>& quadtree_planets,std::uint32_t frame,std::uint32_t own_index,RNG& rng);

    void accelerate(bool is_player,std::uint32_t frame);
    void movement(uint32_t own_index,const Quadtree<double,std::uint32_t>& quadtree_debris,const Quadtree<double,std::uint32_t>& quadtree_shots,
                  const Quadtree<double,std::uint32_t>& quadtree_explosions,const Quadtree<double,std::uint32_t>& quadtree_items,RNG& rng);

    void animate();
    void render(bool tractoring);
};

#endif
