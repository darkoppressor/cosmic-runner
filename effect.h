/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef effect_h
#define effect_h

#include <collision.h>
#include <coords.h>
#include <sprite.h>
#include <math_vector.h>

#include <string>
#include <cstdint>

class Effect{
private:

    Collision_Rect<double> box;

    Vector velocity;

    double angle;
    Vector angular_velocity;

    double scale;

    Sprite sprite;

    bool timed;
    std::uint32_t counter;

public:

    Effect(std::string new_sprite,double new_scale,const Coords<double>& position,std::string sound,const Vector& new_velocity,double new_angle,
           const Vector& new_angular_velocity,std::uint32_t seconds);

    bool is_done() const;

    void countdown();

    void movement();

    void animate();
    void render();
};

#endif
