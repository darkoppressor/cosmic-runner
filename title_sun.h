/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef title_sun_h
#define title_sun_h

#include <collision.h>
#include <coords.h>
#include <sprite.h>
#include <rng.h>

#include <string>
#include <vector>

class Title_Sun{
private:

    Collision_Rect<double> box;

    Sprite sprite;

    std::vector<double> shimmers;
    std::uint32_t shimmer_spawn;

public:

    Title_Sun(std::string new_sprite,const Coords<double>& position);

    void animate(RNG& rng);
    void render();
};

#endif
