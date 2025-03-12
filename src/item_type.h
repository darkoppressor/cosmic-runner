/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef item_type_h
#define item_type_h

#include <string>
#include <cstdint>

class Item_Type {
    public:
        std::string name;
        std::string sprite;
        std::string collect_sound;

        // kg
        double mass;
        // N
        double thrust_decel;
        std::uint64_t point_value;

        Item_Type ();

        bool restores_hull() const;
        bool restores_power() const;
};

#endif
