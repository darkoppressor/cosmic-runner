/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

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
