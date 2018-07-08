/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "ship_type.h"

using namespace std;

Ship_Type::Ship_Type(){
    name="";

    faction="";

    sprite="";

    color="";

    mass=0.0;
    thrust_accel=0.0;
    thrust_decel=0.0;
    max_speed=0.0;

    hull_max=0;
    shields_max=0;

    item_drop_min=0;
    item_drop_max=0;

    point_value=0;

    angular_velocity = Vector(0.0, 0.0);
}
