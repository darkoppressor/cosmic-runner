/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef ship_type_h
#define ship_type_h

#include <collision/collision.h>
#include <math/math_vector.h>

#include <string>
#include <cstdint>
#include <vector>

class Ship_Type {
    public:
        std::string name;
        std::string faction;
        std::string sprite;
        std::string color;
        Collision_Rect<double> box_collision;
        std::vector<std::string> upgrades;

        // kg
        double mass;
        // N
        double thrust_accel;
        double thrust_decel;
        // m/s
        double max_speed;
        std::int32_t hull_max;
        std::int32_t shields_max;
        std::uint32_t item_drop_min;
        std::uint32_t item_drop_max;
        std::uint64_t point_value;
        Vector angular_velocity;

        Ship_Type ();
};

#endif
