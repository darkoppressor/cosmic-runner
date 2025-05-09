/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef shot_h
#define shot_h

#include "shot_type.h"
#include "upgrade.h"

#include <collision/collision.h>
#include <math/math_vector.h>
#include <collision/coords.h>
#include <sprite/sprite.h>
#include <collision/quadtree.h>
#include <rng/rng.h>

#include <string>
#include <cstdint>

class Shot {
    private:
        Collision_Rect<double> box;
        Vector velocity;
        Vector force;
        double angle;
        Vector angular_velocity;
        std::string type;
        std::string faction;
        bool alive;
        std::int32_t owner_index;
        std::string firing_upgrade;
        std::uint32_t homing_delay;
        std::int32_t damage_mod;
        Sprite sprite;
        std::uint32_t smoke_delay;

    public:
        Shot (std::uint32_t new_owner_index, std::string new_type, std::string new_faction,
              std::string new_firing_upgrade, const Coords<double>& position, double new_angle,
              std::int32_t new_damage_mod);

        Shot_Type* get_shot_type() const;

        double get_mass() const;
        std::string get_faction() const;

        Collision_Rect<double> get_box() const;
        Vector get_velocity() const;

        Collision_Rect<double> get_collision_box() const;

        double get_angle() const;

        bool is_alive() const;

        double get_distance_to_player() const;

        bool has_owner() const;
        std::uint32_t get_owner_index() const;
        void clear_owner();
        void notify_of_ship_death(std::uint32_t index);

        Upgrade* get_firing_upgrade() const;
        std::int32_t get_damage() const;

        bool can_home() const;

        void die();

        void cooldown();

        void thrust(const Quadtree<double, std::uint32_t>& quadtree_ships);

        void accelerate(const Quadtree<double, std::uint32_t>& quadtree_ships);
        void movement(RNG& rng, const Quadtree<double, std::uint32_t>& quadtree_debris);

        void animate(RNG& rng);
        void render();
};

#endif
