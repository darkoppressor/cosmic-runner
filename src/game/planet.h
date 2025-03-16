/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef planet_h
#define planet_h

#include "planet_type.h"

#include <collision/collision.h>
#include <sprite/sprite.h>
#include <collision/coords.h>
#include <rng/rng.h>

#include <string>
#include <cstdint>

class Planet {
    private:
        Collision_Circ<double> circle;
        std::string type;
        Sprite sprite;
        std::uint32_t cloud_delay;

    public:
        Planet (std::string new_type, const Coords<double>& position);

        Planet_Type* get_planet_type() const;

        Collision_Circ<double> get_circle() const;

        void animate(RNG& rng);
        void render();
};

#endif
