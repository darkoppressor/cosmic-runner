/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef title_tree_h
#define title_tree_h

#include <collision/collision.h>
#include <collision/coords.h>
#include <sprite/sprite.h>
#include <rng/rng.h>

#include <string>

class Title_Tree {
    private:
        Collision_Rect<double> box;
        double distance_scale;
        Sprite sprite;
        std::uint32_t sprite_animation_speed;

    public:
        Title_Tree (RNG& rng, const Coords<double>& position, double new_distance_scale);

        void animate();
        void render();
};

#endif
