/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef star_h
#define star_h

#include <collision.h>
#include <coords.h>

class Star{
private:

    Collision_Circ<double> circle;

public:

    Star(const Coords<double>& position);

    Collision_Circ<double> get_circle() const;

    void render();
};

#endif
