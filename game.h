/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef game_h
#define game_h

#include "ship.h"
#include "debris.h"
#include "effect.h"

#include <quadtree.h>
#include <rng.h>
#include <coords.h>

#include <vector>
#include <cstdint>

class Game{
private:

    static std::vector<Ship> ships;
    static std::vector<Debris> debris;
    static std::vector<Effect> effects;

    static RNG rng;

    static Ship& get_player();

public:

    //pixels
    static double world_width;
    static double world_height;

    static std::uint64_t score;
    static std::uint64_t score_multiplier;

    //contract target planet index
    static std::int32_t contract;

    static Quadtree<double,std::uint32_t> quadtree_debris;

    static void clear_world();
    static void generate_world();

    static std::uint32_t get_effect_count();
    static const Ship& get_player_const();
    static const Debris& get_debris(std::uint32_t index);

    static void create_effect(std::string sprite,double scale,const Coords<double>& position,std::string sound);

    static void player_thrust(std::string direction);
    static void player_brake(bool brake);

    static void tick();
    static void ai();
    static void movement();
    static void events();
    static void animate();
    static void render();

    static void render_to_textures();

    static void update_background();
    static void render_background();

    static bool move_input_state(std::string direction);
};

#endif
