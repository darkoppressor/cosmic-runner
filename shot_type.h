/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

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
