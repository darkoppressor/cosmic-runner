/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef effect_h
#define effect_h

#include <collision.h>
#include <coords.h>
#include <sprite.h>

#include <string>

class Effect{
private:

    Collision_Rect<double> box;

    double scale;

    Sprite sprite;

    std::string sound;

public:

    Effect(std::string new_sprite,double new_scale,const Coords<double>& position,std::string new_sound);

    bool is_done() const;

    void play_sound();

    void animate();
    void render();
};

#endif
