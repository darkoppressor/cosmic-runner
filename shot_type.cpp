/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "shot_type.h"

using namespace std;

Shot_Type::Shot_Type(){
    name="";

    sprite="";

    death_sprite="";
    death_sound="";

    collision_percentage=0.0;

    mass=0.0;

    damage_type="";

    homing=false;
}
