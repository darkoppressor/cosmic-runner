/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "upgrade.h"

using namespace std;

Upgrade::Upgrade(){
    name="";
    display_name="";
    description="";

    type="";

    banned=false;

    sound="";

    cooldown=0;

    shot_type="";
    spread_type="";
    thrust_accel=0.0;
    max_speed=0.0;
    damage=0;
    shots=0;
    range=0.0;

    power_use=0;
    off_sound="";

    max_shields=0;
    shield_recharge_rate_up=0;
    shield_recharge_rate_down=0;
    max_hull=0;
    thrust_decel=0.0;
    cooldown_up=0;
    cooldown_down=0;
    solid_damage_mod=0;
    explosive_damage_mod=0;
    energy_damage_mod=0;
    point_defense=false;
}

bool Upgrade::is_weapon() const{
    return type=="weapon";
}

bool Upgrade::is_active() const{
    return type=="active";
}

bool Upgrade::is_passive() const{
    return type=="passive";
}

bool Upgrade::spreads_normally() const{
    return spread_type=="normal";
}

bool Upgrade::spreads_evenly() const{
    return spread_type=="even";
}

bool Upgrade::gives_point_defense() const{
    return is_passive() && point_defense;
}
