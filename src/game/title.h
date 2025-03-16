/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef title_h
#define title_h

#include "title_ship.h"
#include "title_sun.h"
#include "title_tree.h"

#include <rng/rng.h>

#include <vector>
#include <cstdint>

class Title {
    private:
        std::vector<Title_Sun> suns;
        std::vector<Title_Ship> ships;
        std::vector<Title_Tree> trees;
        std::uint32_t ship_spawn_check;

    public:
        Title ();

        void clear_title();
        void setup(RNG& rng);

        void generate_ship(RNG& rng);

        void movement();
        void check_ships(RNG& rng);

        void animate(RNG& rng);
        void render();
};

#endif
