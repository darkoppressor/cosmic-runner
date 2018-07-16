/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef title_tree_h
#define title_tree_h

#include <collision.h>
#include <coords.h>
#include <sprite.h>
#include <rng.h>

#include <string>

class Title_Tree {
private:

    Collision_Rect<double> box;

    double distance_scale;

    Sprite sprite;
    std::uint32_t sprite_animation_speed;

public:

    Title_Tree(RNG& rng, const Coords<double>& position,double new_distance_scale);

    void animate();
    void render();
};

#endif
