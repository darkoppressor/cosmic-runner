/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef debris_h
#define debris_h

#include "debris_type.h"

#include <collision.h>
#include <coords.h>
#include <sprite.h>
#include <math_vector.h>

#include <string>

class Debris{
private:

    Collision_Rect<double> box;

    double angle;
    Vector angular_velocity;

    std::string type;

    Sprite sprite;

public:

    Debris(std::string new_type,const Coords<double>& position,double new_angle,const Vector& new_angular_velocity);

    Debris_Type* get_debris_type() const;

    Collision_Rect<double> get_box() const;

    Collision_Rect<double> get_collision_box() const;

    double get_angle() const;
    Vector get_angular_velocity() const;

    void rotation();

    void animate();
    void render();
};

#endif
