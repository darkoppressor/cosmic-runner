/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "item_type.h"

using namespace std;

Item_Type::Item_Type () {
    name = "";

    sprite = "";

    collect_sound = "";

    mass = 0.0;
    thrust_decel = 0.0;

    point_value = 0;
}

bool Item_Type::restores_hull () const {
    return name == "restore_hull";
}

bool Item_Type::restores_power () const {
    return name == "restore_power";
}
