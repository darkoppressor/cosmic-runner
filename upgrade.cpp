/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "upgrade.h"

using namespace std;

Upgrade::Upgrade(){
    name="";
    display_name="";

    type="";

    sound="";

    cooldown=0;

    shot_type="";
    spread_type="";
    thrust_accel=0.0;
    max_speed=0.0;
    damage=0;
    shots=0;
    range=0.0;
}

bool Upgrade::is_weapon() const{
    return type=="weapon";
}

bool Upgrade::is_active() const{
    return type=="active";
}

bool Upgrade::spreads_normally() const{
    return spread_type=="normal";
}

bool Upgrade::spreads_evenly() const{
    return spread_type=="even";
}
