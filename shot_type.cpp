/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "shot_type.h"

using namespace std;

Shot_Type::Shot_Type () {
    name = "";

    sprite = "";

    death_sprite = "";
    death_sound = "";

    collision_percentage = 0.0;

    mass = 0.0;

    damage_type = "";

    homing = false;
}
