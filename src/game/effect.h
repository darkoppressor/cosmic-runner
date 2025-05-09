/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef effect_h
#define effect_h

#include <collision/collision.h>
#include <collision/coords.h>
#include <sprite/sprite.h>
#include <math/math_vector.h>

#include <string>
#include <cstdint>

class Effect {
    private:
        bool done;
        double opacity;
        bool fade;

        // If line is true:
        // box.x,box.y == start point
        // box.w,box.h == end point
        Collision_Rect<double> box;
        Vector velocity;
        double angle;
        Vector angular_velocity;
        double scale;
        bool cast_shadow;
        Sprite sprite;
        bool timed;
        std::uint32_t counter;
        bool line;
        std::string text;
        std::string color;
        bool cloud;
        bool cloud_fading;

    public:
        Effect (std::string new_sprite, bool new_fade, double new_scale, const Coords<double>& position,
                std::string sound, const Vector& new_velocity, double new_angle, const Vector& new_angular_velocity,
                std::uint32_t seconds, bool new_line, const Coords<double>& end_point, std::string new_color,
                std::string new_text, bool new_cloud);

        bool is_fading() const;
        bool is_done() const;

        void countdown();

        void movement();

        void animate();
        void render(bool clouds);
};

#endif
