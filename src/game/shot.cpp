/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "shot.h"
#include "game_data.h"
#include "game.h"
#include "game_constants.h"
#include "../options/game_options.h"

#include <engine.h>
#include <game/game_manager.h>
#include <render/render.h>
#include <strings/engine_strings.h>

#include <unordered_set>

using namespace std;

Shot::Shot (uint32_t new_owner_index, string new_type, string new_faction, string new_firing_upgrade,
            const Coords<double>& position, double new_angle, int32_t new_damage_mod) {
    owner_index = (int32_t) new_owner_index;

    type = new_type;

    faction = new_faction;

    box.x = position.x;
    box.y = position.y;

    angle = new_angle;

    sprite.set_name(get_shot_type()->sprite);

    box.w = sprite.get_width();
    box.h = sprite.get_height();

    alive = true;

    firing_upgrade = new_firing_upgrade;

    homing_delay = 0;

    if (get_shot_type()->homing) {
        homing_delay = Game_Constants::MISSILE_HOMING_DELAY * Engine::UPDATE_RATE / 1000;
    }

    damage_mod = new_damage_mod;

    smoke_delay = 0;

    if (get_shot_type()->damage_type == "explosive") {
        smoke_delay = Game_Constants::SMOKE_DELAY * Engine::UPDATE_RATE / 1000;
    }
}

Shot_Type* Shot::get_shot_type () const {
    return Game_Data::get_shot_type(type);
}

double Shot::get_mass () const {
    return get_shot_type()->mass;
}

string Shot::get_faction () const {
    return faction;
}

Collision_Rect<double> Shot::get_box () const {
    return box;
}

Vector Shot::get_velocity () const {
    return velocity;
}

Collision_Rect<double> Shot::get_collision_box () const {
    return Collision_Rect<double>(box.x + box.w / 2.0 - box.w * get_shot_type()->collision_percentage / 2.0,
                                  box.y + box.h / 2.0 - box.h * get_shot_type()->collision_percentage / 2.0,
                                  box.w * get_shot_type()->collision_percentage,
                                  box.h * get_shot_type()->collision_percentage);
}

double Shot::get_angle () const {
    return angle;
}

bool Shot::is_alive () const {
    return alive;
}

double Shot::get_distance_to_player () const {
    const Ship& player = Game::get_player_const();

    return Math::get_distance_between_points(box.get_center(), player.get_box().get_center());
}

bool Shot::has_owner () const {
    return owner_index >= 0;
}

uint32_t Shot::get_owner_index () const {
    return (uint32_t) owner_index;
}

void Shot::clear_owner () {
    owner_index = -1;
}

void Shot::notify_of_ship_death (uint32_t index) {
    if (has_owner()) {
        if (get_owner_index() == index) {
            clear_owner();
        } else if (index < get_owner_index()) {
            owner_index--;
        }
    }
}

Upgrade* Shot::get_firing_upgrade () const {
    return Game_Data::get_upgrade_type(firing_upgrade);
}

int32_t Shot::get_damage () const {
    int32_t damage = get_firing_upgrade()->damage + damage_mod;

    if (damage < 1) {
        damage = 1;
    }

    return damage;
}

bool Shot::can_home () const {
    return homing_delay == 0 && get_shot_type()->homing;
}

void Shot::die () {
    if (is_alive()) {
        alive = false;

        if (get_shot_type()->death_sprite.length() > 0) {
            Game::create_effect(get_shot_type()->death_sprite, false, 1.0,
                                Coords<double>(box.center_x(), box.center_y()), get_shot_type()->death_sound, Vector(),
                                angle, Vector(), 0, false, Coords<double>());
        }
    }
}

void Shot::cooldown () {
    if (is_alive()) {
        if (homing_delay > 0) {
            homing_delay--;
        }
    }
}

void Shot::thrust (const Quadtree<double, uint32_t>& quadtree_ships) {
    if (can_home()) {
        Collision_Rect<double> box_targeting = box;

        box_targeting.x -= Game_Constants::MISSILE_HOMING_RANGE;
        box_targeting.y -= Game_Constants::MISSILE_HOMING_RANGE;
        box_targeting.w += Game_Constants::MISSILE_HOMING_RANGE * 2.0;
        box_targeting.h += Game_Constants::MISSILE_HOMING_RANGE * 2.0;

        vector<uint32_t> nearby_ships;

        quadtree_ships.get_objects(nearby_ships, box_targeting);

        vector<uint32_t> valid_targets;
        unordered_set<uint32_t> collisions;

        // Find all valid targets
        for (size_t i = 0; i < nearby_ships.size(); i++) {
            if (!collisions.count(nearby_ships[i])) {
                collisions.emplace(nearby_ships[i]);

                const Ship& ship = Game::get_ship(nearby_ships[i]);

                if (ship.is_alive() && !ship.is_landing() && (nearby_ships[i] != 0 || !Game::player_is_landed()) &&
                    !ship.is_cloaked() && !ship.is_warping()) {
                    Collision_Rect<double> box_ship = ship.get_box();

                    if (faction != ship.get_faction() && Collision::check_rect(box_targeting, box_ship)) {
                        valid_targets.push_back(nearby_ships[i]);
                    }
                }
            }
        }

        // Find nearest valid target
        int32_t nearest_index = -1;
        double nearest_distance = 0.0;

        for (size_t i = 0; i < valid_targets.size(); i++) {
            const Ship& ship = Game::get_ship(valid_targets[i]);
            double new_distance = Math::get_distance_between_points(box.get_center(), ship.get_box().get_center());

            if (nearest_index == -1 || new_distance < nearest_distance) {
                nearest_index = valid_targets[i];
                nearest_distance = new_distance;
            }
        }

        // If a nearest valid target was found
        if (nearest_index >= 0) {
            const Ship& ship = Game::get_ship((uint32_t) nearest_index);

            angle = box.get_angle_to_rect(ship.get_box());
        }
    }

    Vector thrust_force(get_firing_upgrade()->thrust_accel, angle);

    force += thrust_force;
}

void Shot::accelerate (const Quadtree<double, uint32_t>& quadtree_ships) {
    if (is_alive()) {
        thrust(quadtree_ships);

        Vector acceleration = force / get_mass();

        velocity += acceleration;

        if (velocity.magnitude > get_firing_upgrade()->max_speed) {
            velocity.magnitude = get_firing_upgrade()->max_speed;
        }

        force *= 0.0;
    }
}

void Shot::movement (RNG& rng, const Quadtree<double, uint32_t>& quadtree_debris) {
    if (is_alive()) {
        Vector_Components vc = velocity.get_components();

        box.x += vc.a / (double) Engine::UPDATE_RATE;
        box.y += vc.b / (double) Engine::UPDATE_RATE;

        Collision_Rect<double> box_collision = get_collision_box();
        vector<uint32_t> nearby_debris;

        quadtree_debris.get_objects(nearby_debris, box_collision);

        unordered_set<uint32_t> collisions;

        for (size_t i = 0; i < nearby_debris.size(); i++) {
            if (is_alive()) {
                if (!collisions.count(nearby_debris[i])) {
                    collisions.emplace(nearby_debris[i]);

                    const Debris& debris = Game::get_debris(nearby_debris[i]);
                    Collision_Rect<double> box_debris = debris.get_collision_box();

                    if (Collision::check_rect_rotated(box_collision, box_debris, angle, debris.get_angle())) {
                        if (get_shot_type()->damage_type == "explosive") {
                            Game::create_explosion("explosions/explosion_missile",
                                                   "explosions/explosion_missile_" +
                                                   Strings::num_to_string(rng.random_range(0, 2)),
                                                   Coords<double>(box.center_x(), box.center_y()), get_damage(),
                                                   faction);
                        }

                        die();
                    }
                }
            } else {
                return;
            }
        }
    }
}

void Shot::animate (RNG& rng) {
    if (is_alive()) {
        sprite.animate();

        if (sprite.frame == 0) {
            sprite.frame++;
        }

        if (get_shot_type()->damage_type == "explosive") {
            if (smoke_delay > 0) {
                smoke_delay--;
            }

            if (smoke_delay == 0) {
                smoke_delay = Game_Constants::SMOKE_DELAY * Engine::UPDATE_RATE / 1000;

                vector<Coords<double>> vertices;

                get_box().get_vertices(vertices, angle);

                Coords<double> smoke_location(rng.random_range(vertices[Collision::VERTEX_UPPER_LEFT].x,
                                                               vertices[Collision::VERTEX_LOWER_LEFT].x),
                                              rng.random_range(vertices[Collision::VERTEX_UPPER_LEFT].y,
                                                               vertices[Collision::VERTEX_LOWER_LEFT].y));

                Game::create_effect("effects/effect_smoke_" + Strings::num_to_string(rng.random_range(0, 2)), true,
                                    0.1 * (double) rng.random_range(5, 15), smoke_location, "",
                                    Vector(rng.random_range(0, 10), rng.random_range(0, 359)), rng.random_range(0, 359),
                                    Vector(0.01 * rng.random_range(0, 50), rng.random_range(0, 359)),
                                    Game_Constants::EFFECT_LENGTH_SMOKE, false, Coords<double>(), "white");
            }
        }
    }
}

void Shot::render () {
    if (is_alive()) {
        if (Collision::check_rect_rotated(box * Game_Manager::camera_zoom, Game_Manager::camera, angle, 0.0)) {
            sprite.render(box.x * Game_Manager::camera_zoom - Game_Manager::camera.x,
                          box.y * Game_Manager::camera_zoom - Game_Manager::camera.y, 1.0, 1.0, 1.0, angle);

            if (Game_Options::show_collision_outlines) {
                Collision_Rect<double> col_box = get_collision_box();

                // Render extra collision boxes
                /*Render::render_rectangle(box.x*Game_Manager::camera_zoom-Game_Manager::camera.x,box.y*Game_Manager::camera_zoom-Game_Manager::camera.y,box.w,box.h,0.25,"white");
                   Render::render_rectangle(col_box.x*Game_Manager::camera_zoom-Game_Manager::camera.x,col_box.y*Game_Manager::camera_zoom-Game_Manager::camera.y,col_box.w,col_box.h,0.25,"red");*/
                vector<Coords<double>> vertices;

                col_box.get_vertices(vertices, angle);

                for (size_t i = 0; i < vertices.size(); i++) {
                    uint32_t start_vertex = (uint32_t) i;
                    uint32_t end_vertex = (i < vertices.size() - 1) ? start_vertex + 1 : 0;

                    Render::render_line(vertices[start_vertex].x * Game_Manager::camera_zoom - Game_Manager::camera.x,
                                        vertices[start_vertex].y * Game_Manager::camera_zoom - Game_Manager::camera.y,
                                        vertices[end_vertex].x * Game_Manager::camera_zoom - Game_Manager::camera.x,
                                        vertices[end_vertex].y * Game_Manager::camera_zoom - Game_Manager::camera.y,
                                        1.0, "red");
                }
            }
        }
    }
}
