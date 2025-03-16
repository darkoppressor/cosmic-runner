/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef debris_type_h
#define debris_type_h

#include <string>
#include <cstdint>

class Debris_Type {
    public:
        std::string name;
        std::string sprite;
        double collision_percentage;
        std::int32_t damage;
        std::string damage_type;

        Debris_Type ();
};

#endif
