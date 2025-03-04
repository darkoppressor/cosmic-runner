/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

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
