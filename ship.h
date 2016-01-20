/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef ship_h
#define ship_h

#include "ship_type.h"

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

    Sprite sprite;

public:

    Ship(std::string new_type,const Coords<double>& position,double new_angle,const std::vector<std::string>& starting_upgrades);

    Ship_Type* get_ship_type() const;

    Collision_Rect<double> get_box() const;
    Vector get_velocity() const;
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

    double get_thrust_accel() const;
    double get_thrust_decel() const;
    double get_max_speed() const;
    std::int32_t get_hull_max() const;
    std::int32_t get_shields_max() const;

    Collision_Rect<double> get_collision_box() const;

    bool is_alive() const;

    double get_distance_to_player() const;

    void take_damage(bool is_player,std::int32_t damage,std::string damage_type,const Coords<double>& location);

    void set_thrust_angle(std::string direction);
    void set_braking(bool new_braking);

    void thrust(std::uint32_t frame);
    void brake(std::uint32_t frame);

    void commence_landing(std::uint32_t new_landing_planet_index);
    bool is_landing();
    void land(bool is_player);

    void regenerate_shields();
    void cooldown(const Quadtree<double,std::uint32_t>& quadtree_ships,RNG& rng,std::uint32_t own_index);

    //Returns true if the weapon found a target and fired upon it
    bool fire_weapon(const Quadtree<double,std::uint32_t>& quadtree_ships,RNG& rng,std::uint32_t own_index);

    void accelerate(bool is_player,std::uint32_t frame);
    void movement(uint32_t own_index,const Quadtree<double,std::uint32_t>& quadtree_debris,const Quadtree<double,std::uint32_t>& quadtree_shots,RNG& rng);

    void animate();
    void render();
};

#endif
