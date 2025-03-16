/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef game_options_h
#define game_options_h

#include <string>
#include <cstdint>

class Game_Options {
    public:
        static std::uint32_t background_layers_stars;
        static std::uint32_t background_layers_planetary;
        static bool show_collision_outlines;
        static double minimap_opacity;

        static bool get_option(std::string name, std::string& value);
        static void set_option(std::string name, std::string value);
};

#endif
