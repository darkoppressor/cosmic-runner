/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "explosion.h"
#include "ship.h"
#include "game.h"
#include "game_data.h"
#include "../options/game_options.h"

#include <game/game_manager.h>
#include <sound/sound_manager.h>
#include <render/render.h>

using namespace std;

Explosion::Explosion (string new_sprite, string sound, const Coords<double>& position, int32_t new_damage,
                      string new_faction, bool new_scan, bool new_emp) {
    circle.x = position.x;
    circle.y = position.y;

    sprite.set_name(new_sprite);

    circle.r = sprite.get_width() / 2.0;

    damage = new_damage;

    faction = new_faction;

    scan = new_scan;
    emp = new_emp;

    if (sound.length() > 0) {
        Sound_Manager::play_sound(sound, circle.x, circle.y);
    }
}

Collision_Circ<double> Explosion::get_circle () const {
    return circle;
}

int32_t Explosion::get_damage () const {
    return damage;
}

string Explosion::get_faction () const {
    return faction;
}

bool Explosion::is_scan () const {
    return scan;
}

bool Explosion::is_emp () const {
    return emp;
}

bool Explosion::is_alive () const {
    return sprite.animating;
}

double Explosion::get_distance_to_player () const {
    const Ship& player = Game::get_player_const();

    return Math::get_distance_between_points(circle.get_center(), player.get_box().get_center());
}

void Explosion::animate () {
    if (is_alive()) {
        sprite.animate();
    }
}

void Explosion::render () {
    if (is_alive()) {
        if (Collision::check_circ_rect(circle * Game_Manager::camera_zoom, Game_Manager::camera)) {
            sprite.render((circle.x - circle.r) * Game_Manager::camera_zoom - Game_Manager::camera.x,
                          (circle.y - circle.r) * Game_Manager::camera_zoom - Game_Manager::camera.y);

            if (Game_Options::show_collision_outlines) {
                Render::render_circle_empty(circle.x * Game_Manager::camera_zoom - Game_Manager::camera.x,
                                            circle.y * Game_Manager::camera_zoom - Game_Manager::camera.y, circle.r,
                                            1.0, "red");
            }
        }
    }
}
