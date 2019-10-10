/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef planet_type_h
#define planet_type_h

#include <string>

class Planet_Type {
    public:
        std::string name;
        std::string sprite;
        std::string minimap_color;
        std::string cloud_color;

        Planet_Type ();
};

#endif
