/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef explosion_h
#define explosion_h

#include "shot.h"

#include <collision.h>
#include <coords.h>
#include <sprite.h>

#include <string>
#include <cstdint>

class Explosion{
private:

    Collision_Circ<double> circle;

    std::int32_t damage;

    std::string faction;

    bool scan;
    bool emp;

    Sprite sprite;

public:

    Explosion(std::string new_sprite,std::string sound,const Coords<double>& position,std::int32_t new_damage,std::string new_faction,bool new_scan,bool new_emp);

    Collision_Circ<double> get_circle() const;

    std::int32_t get_damage() const;

    std::string get_faction() const;

    bool is_scan() const;
    bool is_emp() const;

    bool is_alive() const;

    double get_distance_to_player() const;

    void animate();
    void render();
};

#endif
