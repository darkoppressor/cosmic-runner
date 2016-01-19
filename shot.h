/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef shot_h
#define shot_h

#include "shot_type.h"

#include <collision.h>
#include <math_vector.h>
#include <coords.h>
#include <sprite.h>
#include <quadtree.h>

#include <string>
#include <cstdint>

class Shot{
private:

    Collision_Rect<double> box;

    Vector velocity;
    Vector force;

    double angle;
    Vector angular_velocity;

    std::string type;

    bool alive;

    Sprite sprite;

public:

    Shot(std::string new_type,const Coords<double>& position,double new_angle);

    Shot_Type* get_shot_type() const;

    Collision_Rect<double> get_box() const;
    Vector get_velocity() const;

    double get_thrust_accel() const;
    double get_max_speed() const;

    Collision_Rect<double> get_collision_box() const;

    bool is_alive() const;

    void die();

    void thrust();

    void accelerate();
    void movement(const Quadtree<double,std::uint32_t>& quadtree_debris);

    void animate();
    void render();
};

#endif
