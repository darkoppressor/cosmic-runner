/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef planet_h
#define planet_h

#include "planet_type.h"

#include <collision.h>
#include <sprite.h>
#include <coords.h>
#include <rng.h>

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
