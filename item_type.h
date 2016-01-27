/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef item_type_h
#define item_type_h

#include <string>
#include <cstdint>

class Item_Type{
public:

    std::string name;

    std::string sprite;

    std::string collect_sound;

    //kg
    double mass;
    //N
    double thrust_decel;

    std::uint64_t point_value;

    Item_Type();

    bool restores_hull() const;
    bool restores_power() const;
};

#endif
