/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "ship_type.h"

using namespace std;

Ship_Type::Ship_Type () {
    name = "";

    faction = "";

    sprite = "";

    color = "";

    mass = 0.0;
    thrust_accel = 0.0;
    thrust_decel = 0.0;
    max_speed = 0.0;

    hull_max = 0;
    shields_max = 0;

    item_drop_min = 0;
    item_drop_max = 0;

    point_value = 0;

    angular_velocity = Vector(0.0, 0.0);
}
