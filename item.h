/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef item_h
#define item_h

#include "item_type.h"

#include <collision.h>
#include <coords.h>
#include <sprite.h>
#include <math_vector.h>

#include <string>

class Item {
    private:
        Collision_Rect<double> box;
        Vector velocity;
        Vector force;
        double angle;
        Vector angular_velocity;
        std::string type;
        bool alive;
        Sprite sprite;

    public:
        Item (std::string new_type, const Coords<double>& position, const Vector& new_velocity, double new_angle,
              const Vector& new_angular_velocity);

        Item_Type* get_item_type() const;

        double get_mass() const;

        Collision_Rect<double> get_box() const;

        bool is_alive() const;

        void die();

        double get_distance_to_player() const;

        void play_collection_sound() const;

        // Returns true if the item was vacuumed towards the player
        // Returns false if the player was not in vacuuming range
        bool vacuum();
        void brake();
        void accelerate();
        void rotation();
        void movement();

        void animate();
        void render();
};

#endif
