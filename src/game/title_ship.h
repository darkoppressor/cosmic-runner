/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef title_ship_h
#define title_ship_h

#include <math/math_vector.h>
#include <collision/collision.h>
#include <collision/coords.h>
#include <sprite/sprite.h>

#include <string>

class Title_Ship {
    private:
        Collision_Rect<double> box;
        Vector velocity;
        double distance_scale;
        Sprite sprite;

    public:
        Title_Ship (std::string new_sprite, const Coords<double>& position, const Vector& new_velocity,
                    double new_distance_scale);

        bool is_alive();

        void movement();
        void animate();
        void render(bool bird);
};

#endif
