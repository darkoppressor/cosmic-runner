/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef hud_h
#define hud_h

#include <coords.h>
#include <sprite.h>

#include <string>

class Hud{
private:

    static Sprite notoriety_sprite;

public:

    static void setup();

    static void animate();

    static void render_bar(std::string font_name,std::string font_color,std::string bar_color,const Coords<double>& position,double a,double b,bool show_background);

    static void render();
};

#endif
