/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef ship_type_h
#define ship_type_h

#include <string>
#include <cstdint>

class Ship_Type{
public:

    std::string name;

    std::string sprite;

    double collision_percentage;

    //kg
    double mass;
    //N
    double thrust_accel;
    double thrust_decel;
    //m/s
    double max_speed;

    std::int32_t hull_max;
    std::int32_t shields_max;

    Ship_Type();
};

#endif
