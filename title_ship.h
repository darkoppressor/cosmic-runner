/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef title_ship_h
#define title_ship_h

#include <math_vector.h>
#include <collision.h>
#include <coords.h>
#include <sprite.h>

#include <string>

class Title_Ship{
private:

    Collision_Rect<double> box;

    Vector velocity;

    double distance_scale;

    Sprite sprite;

public:

    Title_Ship(std::string new_sprite,const Coords<double>& position,const Vector& new_velocity,double new_distance_scale);

    bool is_alive();

    void movement();
    void animate();
    void render();
};

#endif
