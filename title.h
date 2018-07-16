/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef title_h
#define title_h

#include "title_ship.h"
#include "title_sun.h"
#include "title_tree.h"

#include <rng.h>

#include <vector>
#include <cstdint>

class Title{
private:

    std::vector<Title_Sun> suns;
    std::vector<Title_Ship> ships;
    std::vector<Title_Tree> trees;

    std::uint32_t ship_spawn_check;

public:

    Title();

    void clear_title();
    void setup(RNG& rng);

    void generate_ship(RNG& rng);

    void movement();
    void check_ships(RNG& rng);

    void animate(RNG& rng);
    void render();
};

#endif
