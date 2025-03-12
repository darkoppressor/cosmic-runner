/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef shot_type_h
#define shot_type_h

#include <string>

class Shot_Type {
    public:
        std::string name;
        std::string sprite;
        std::string death_sprite;
        std::string death_sound;
        double collision_percentage;

        // kg
        double mass;
        std::string damage_type;
        bool homing;

        Shot_Type ();
};

#endif
