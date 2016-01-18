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

    Sprite sprite;

public:

    Ship(std::string new_type,const Coords<double>& position,double new_angle);

    Ship_Type* get_ship_type() const;

    Collision_Rect<double> get_box() const;
    Vector get_velocity() const;
    std::int32_t get_hull() const;
    std::int32_t get_shields() const;

    Collision_Rect<double> get_collision_box() const;

    bool is_alive() const;

    void take_damage(std::int32_t damage,std::string damage_type,const Coords<double>& location);

    void set_thrust_angle(std::string direction);
    void set_braking(bool new_braking);

    void thrust();
    void brake();

    void accelerate();
    void movement(const Quadtree<double,std::uint32_t>& quadtree_debris,RNG& rng);

    void animate();
    void render();
};

#endif
