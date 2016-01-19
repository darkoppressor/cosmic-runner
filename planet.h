/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef planet_h
#define planet_h

#include <collision.h>
#include <sprite.h>
#include <coords.h>

#include <string>

class Planet{
private:

    Collision_Circ<double> circle;

    Sprite sprite;

public:

    Planet(std::string new_sprite,const Coords<double>& position);

    Collision_Circ<double> get_circle() const;

    void animate();
    void render();
};

#endif
