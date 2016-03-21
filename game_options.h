/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef game_options_h
#define game_options_h

#include <string>

class Game_Options{
public:

    static uint32_t background_layers_stars;
    static uint32_t background_layers_planetary;

    static bool show_collision_outlines;

    static double minimap_opacity;

    static bool get_option(std::string name,std::string& value);
    static void set_option(std::string name,std::string value);
};

#endif
