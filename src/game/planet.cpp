/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "planet.h"
#include "../options/game_options.h"
#include "game_constants.h"
#include "game.h"
#include "game_data.h"

#include <game/game_manager.h>
#include <render/render.h>
#include <engine.h>
#include <math/engine_math.h>
#include <strings/engine_strings.h>

using namespace std;

Planet::Planet (string new_type, const Coords<double>& position) {
    type = new_type;

    sprite.set_name(get_planet_type()->sprite);

    circle.x = position.x;
    circle.y = position.y;
    circle.r = sprite.get_width() / 2.0;

    cloud_delay = Game_Constants::CLOUD_DELAY * Engine::UPDATE_RATE / 1000;
}

Planet_Type* Planet::get_planet_type () const {
    return Game_Data::get_planet_type(type);
}

Collision_Circ<double> Planet::get_circle () const {
    return circle;
}

void Planet::animate (RNG& rng) {
    sprite.animate();

    if (!Collision::check_circ_rect(circle * Game_Manager::camera_zoom, Game_Manager::camera)) {
        if (cloud_delay > 0) {
            cloud_delay--;
        }

        if (cloud_delay == 0) {
            cloud_delay = Game_Constants::CLOUD_DELAY * Engine::UPDATE_RATE / 1000;

            if (rng.random_range(0, 99) < Game_Constants::CLOUD_CHANCE) {
                Coords<double> location;
                double angle = 0.0;

                if (rng.random_range(0, 1)) {
                    location = Math::rotate_point(Coords<double>(circle.get_center().x + circle.r,
                                                                 circle.get_center().y), circle.get_center(),
                                                  rng.random_range(135, 225));
                } else {
                    if (rng.random_range(0, 1)) {
                        location = Math::rotate_point(Coords<double>(circle.get_center().x + circle.r,
                                                                     circle.get_center().y), circle.get_center(),
                                                      rng.random_range(0, 45));
                    } else {
                        location = Math::rotate_point(Coords<double>(circle.get_center().x + circle.r,
                                                                     circle.get_center().y), circle.get_center(),
                                                      rng.random_range(315, 360));
                    }

                    angle = 180.0;
                }

                Game::create_effect("effects/effect_cloud_" + Strings::num_to_string(rng.random_range(0, 2)), true,
                                    0.1 * (double) rng.random_range(5, 30), location, "",
                                    Vector(rng.random_range(5, 15), angle), 0.0, Vector(0.0, 0.0), 0, false,
                                    Coords<double>(), get_planet_type()->cloud_color, "", true);
            }
        }
    }
}

void Planet::render () {
    if (Collision::check_circ_rect(circle * Game_Manager::camera_zoom, Game_Manager::camera)) {
        sprite.render((circle.x - circle.r) * Game_Manager::camera_zoom - Game_Manager::camera.x,
                      (circle.y - circle.r) * Game_Manager::camera_zoom - Game_Manager::camera.y);

        if (Game_Options::show_collision_outlines) {
            Render::render_circle_empty(circle.x * Game_Manager::camera_zoom - Game_Manager::camera.x,
                                        circle.y * Game_Manager::camera_zoom - Game_Manager::camera.y, circle.r, 1.0,
                                        "red");
        }
    }
}
