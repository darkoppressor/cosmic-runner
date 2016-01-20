/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef upgrade_h
#define upgrade_h

#include <string>

class Upgrade{
public:

    std::string name;
    std::string display_name;

    std::string type;

    std::string sound;

    //ms
    std::uint32_t cooldown;

    //Weapon-specific values:
    std::string shot_type;
    std::string spread_type;
    //N
    double thrust_accel;
    //m/s
    double max_speed;
    std::int32_t damage;
    std::uint32_t shots;
    //m (pixels)
    double range;

    Upgrade();

    bool is_weapon() const;
    bool is_active() const;

    bool spreads_normally() const;
    bool spreads_evenly() const;
};

#endif
