/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef upgrade_h
#define upgrade_h

#include <string>
#include <cstdint>

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
    //Also used by passives
    double thrust_accel;
    //m/s
    //Also used by passives
    double max_speed;
    std::int32_t damage;
    std::uint32_t shots;
    //m (pixels)
    double range;

    //Passive-specific values:
    std::int32_t max_shields;

    //ms
    //As the names imply, increasing the rate means shields recharge faster, while decreasing the rate means shields recharge slower
    std::uint32_t shield_recharge_rate_up;
    std::uint32_t shield_recharge_rate_down;

    std::int32_t max_hull;
    //N
    double thrust_decel;

    //ms
    //As the names imply, increasing the cooldown means the thing takes longer to be ready for use,
    //while decreasing the cooldown means the thing is ready for use more quickly
    std::uint32_t cooldown_up;
    std::uint32_t cooldown_down;

    std::int32_t solid_damage_mod;
    std::int32_t explosive_damage_mod;
    std::int32_t energy_damage_mod;
    bool point_defense;

    Upgrade();

    bool is_weapon() const;
    bool is_active() const;
    bool is_passive() const;

    bool spreads_normally() const;
    bool spreads_evenly() const;

    bool gives_point_defense() const;
};

#endif
