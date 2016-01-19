/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef shot_type_h
#define shot_type_h

#include <string>
#include <cstdint>

class Shot_Type{
public:

    std::string name;

    std::string sprite;

    double collision_percentage;

    //kg
    double mass;
    //N
    double thrust_accel;
    //m/s
    double max_speed;

    std::int32_t damage;
    std::string damage_type;

    Shot_Type();
};

#endif
