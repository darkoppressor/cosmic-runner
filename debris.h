/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef debris_h
#define debris_h

#include "debris_type.h"

#include <collision.h>
#include <coords.h>
#include <sprite.h>
#include <math_vector.h>

#include <string>

class Debris {
    private:
        Collision_Rect<double> box;
        double angle;
        Vector angular_velocity;
        std::string type;
        bool cast_shadow;
        Sprite sprite;

    public:
        Debris (std::string new_type, const Coords<double>& position, double new_angle,
                const Vector& new_angular_velocity);

        Debris_Type* get_debris_type() const;

        Collision_Rect<double> get_box() const;

        Collision_Rect<double> get_collision_box() const;

        double get_angle() const;
        Vector get_angular_velocity() const;

        void rotation();

        void animate();
        void render();
};

#endif
