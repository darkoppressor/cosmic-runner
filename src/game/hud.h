/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef hud_h
#define hud_h

#include <collision/coords.h>
#include <sprite/sprite.h>

#include <string>

class Hud {
    private:
        static Sprite notoriety_sprite;

    public:
        static void setup();
        static void animate();
        static void render_bar(std::string font_name, std::string font_color, std::string bar_color,
                               const Coords<double>& position, double a, double b, bool show_background);
        static void render();
};

#endif
