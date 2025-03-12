/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "minimap.h"
#include "game_constants.h"
#include "game.h"
#include "game_options.h"

#include <pixels.h>
#include <log.h>
#include <engine.h>
#include <object_manager.h>
#include <render.h>
#include <game_window.h>
#include <engine_strings.h>

#include <SDL.h>

using namespace std;

bool Minimap::is_generated () const {
    return image_data.texture != 0;
}

double Minimap::get_scale_x () const {
    double scale = Game::world_width / (double) width;

    if (scale < 1.0) {
        scale = 1.0;
    }

    return scale;
}

double Minimap::get_scale_y () const {
    double scale = Game::world_height / (double) height;

    if (scale < 1.0) {
        scale = 1.0;
    }

    return scale;
}

Minimap::Minimap () {
    width = 0;
    height = 0;
}

void Minimap::clear_map () {
    if (image_data.texture != 0) {
        image_data.unload_image();
    }
}

void Minimap::generate_map (uint32_t new_width, uint32_t new_height) {
    clear_map();

    width = new_width;
    height = new_height;

    uint32_t rmask = 0;
    uint32_t gmask = 0;
    uint32_t bmask = 0;
    uint32_t amask = 0;

    Engine::get_rgba_masks(&rmask, &gmask, &bmask, &amask);

    SDL_Surface* surface = SDL_CreateRGBSurface(0, (int) width, (int) height, 32, rmask, gmask, bmask, amask);

    if (surface != 0) {
        if (SDL_MUSTLOCK(surface)) {
            if (SDL_LockSurface(surface) != 0) {
                string msg = "Error locking surface to generate minimap: ";

                msg += SDL_GetError();
                Log::add_error(msg);
            }
        }

        for (uint32_t x = 0; x < width; x++) {
            for (uint32_t y = 0; y < height; y++) {
                Color color = *Object_Manager::get_color("space");

                color.set(color.get_red(), color.get_green(), color.get_blue(), (short) 255);

                Pixels::surface_put_pixel(surface, x, y, color);
            }
        }

        uint32_t planets = Game::get_planet_count();

        for (uint32_t i = 0; i < planets; i++) {
            const Planet& planet = Game::get_planet(i);
            int32_t start_x = (planet.get_circle().x - planet.get_circle().r) / get_scale_x();
            int32_t end_x = (planet.get_circle().x + planet.get_circle().r) / get_scale_x();
            int32_t start_y = (planet.get_circle().y - planet.get_circle().r) / get_scale_y();
            int32_t end_y = (planet.get_circle().y + planet.get_circle().r) / get_scale_y();
            int32_t radius = planet.get_circle().r / ((get_scale_x() + get_scale_y()) / 2.0);
            Collision_Circ<int32_t> circle(start_x + radius, start_y + radius, radius);
            Collision_Circ<int32_t> circle_omit = circle;

            circle_omit.r -= Game_Constants::MINIMAP_LINE_THICKNESS;

            for (int32_t x = start_x; x <= end_x; x++) {
                for (int32_t y = start_y; y <= end_y; y++) {
                    if (x >= 0 && x < surface->w && y >= 0 && y < surface->h) {
                        Collision_Circ<int32_t> point(x, y, 0);

                        if (Collision::check_circ(point, circle) && !Collision::check_circ(point, circle_omit)) {
                            Pixels::surface_put_pixel(surface, x, y,
                                                      *Object_Manager::get_color(
                                                          planet.get_planet_type()->minimap_color));
                        }
                    }
                }
            }
        }

        // I think it looks better without the asteroids
        uint32_t debris_count = Game::get_debris_count();

        for (uint32_t i = 0; i < debris_count; i++) {
            const Debris& debris = Game::get_debris(i);
            int32_t start_x = debris.get_box().x / get_scale_x();
            int32_t end_x = (debris.get_box().x + debris.get_box().w) / get_scale_x();
            int32_t start_y = debris.get_box().y / get_scale_y();
            int32_t end_y = (debris.get_box().y + debris.get_box().h) / get_scale_y();
            Collision_Rect<int32_t> box_omit(start_x, start_y, end_x - start_x, end_y - start_y);

            box_omit.x += Game_Constants::MINIMAP_LINE_THICKNESS;
            box_omit.y += Game_Constants::MINIMAP_LINE_THICKNESS;
            box_omit.w -= Game_Constants::MINIMAP_LINE_THICKNESS * 2;
            box_omit.h -= Game_Constants::MINIMAP_LINE_THICKNESS * 2;

            for (int32_t x = start_x; x <= end_x; x++) {
                for (int32_t y = start_y; y <= end_y; y++) {
                    if (x >= 0 && x < surface->w && y >= 0 && y < surface->h) {
                        if (!Collision::check_rect(Collision_Rect<int32_t>(x, y, 0, 0), box_omit)) {
                            Pixels::surface_put_pixel(surface, x, y, *Object_Manager::get_color("minimap_asteroid"));
                        }
                    }
                }
            }
        }

        if (SDL_MUSTLOCK(surface)) {
            SDL_UnlockSurface(surface);
        }

        image_data.load_image(surface);

        SDL_FreeSurface(surface);
    } else {
        string msg = "Error generating minimap: ";

        msg += SDL_GetError();
        Log::add_error(msg);
    }
}

Collision_Rect<double> Minimap::get_box () const {
    return Collision_Rect<double>((double) Game_Window::width() / 2.0 -
                                  ((double) width - Game_Constants::MINIMAP_BORDER_THICKNESS) / 2.0,
                                  (double) Game_Window::height() / 2.0 -
                                  ((double) height - Game_Constants::MINIMAP_BORDER_THICKNESS) / 2.0, (double) width,
                                  (double) height);
}

void Minimap::render () {
    if (is_generated()) {
        Collision_Rect<double> box = get_box();

        // Render the border
        if (Game_Constants::MINIMAP_BORDER_THICKNESS > 0.0) {
            Render::render_rectangle_empty(box.x - Game_Constants::MINIMAP_BORDER_THICKNESS,
                                           box.y - Game_Constants::MINIMAP_BORDER_THICKNESS,
                                           (double) width + Game_Constants::MINIMAP_BORDER_THICKNESS * 2.0,
                                           (double) height + Game_Constants::MINIMAP_BORDER_THICKNESS * 2.0,
                                           Game_Options::minimap_opacity, "minimap_border",
                                           Game_Constants::MINIMAP_BORDER_THICKNESS);
        }

        // Render the texture
        Render::render_texture(box.x, box.y, &image_data, Game_Options::minimap_opacity);

        const Ship& player = Game::get_player_const();
        double player_x = player.get_box().x / get_scale_x();
        double player_y = player.get_box().y / get_scale_y();

        if (Game::player_has_contract()) {
            vector<Coords<double>> vertices;

            player.get_box().get_vertices(vertices, player.get_angle());

            const Planet& planet = Game::get_contract_planet();

            if (!Game::is_object_over_planet(vertices, planet)) {
                Coords<double> planet_position(planet.get_circle().x / get_scale_x(),
                                               planet.get_circle().y / get_scale_y());

                Render::render_line(box.x + player_x, box.y + player_y, box.x + planet_position.x,
                                    box.y + planet_position.y, 1.0, "minimap_arrow");

                double angle = planet.get_circle().get_angle_to_rect(player.get_box());
                Vector draw_vertex_1(Game_Constants::MINIMAP_ARROW_MAGNITUDE,
                                     angle - Game_Constants::MINIMAP_ARROW_ANGLE_MOD);
                Vector draw_vertex_2(Game_Constants::MINIMAP_ARROW_MAGNITUDE,
                                     angle + Game_Constants::MINIMAP_ARROW_ANGLE_MOD);
                Coords<double> vertex_1 = planet_position;
                Coords<double> vertex_2 = planet_position;
                Vector_Components vc_1 = draw_vertex_1.get_components();
                Vector_Components vc_2 = draw_vertex_2.get_components();

                vertex_1.x += vc_1.a;
                vertex_1.y += vc_1.b;

                vertex_2.x += vc_2.a;
                vertex_2.y += vc_2.b;

                Render::render_line(box.x + planet_position.x, box.y + planet_position.y, box.x + vertex_1.x,
                                    box.y + vertex_1.y, 1.0, "minimap_arrow");
                Render::render_line(box.x + planet_position.x, box.y + planet_position.y, box.x + vertex_2.x,
                                    box.y + vertex_2.y, 1.0, "minimap_arrow");
                Render::render_line(box.x + vertex_1.x, box.y + vertex_1.y, box.x + vertex_2.x, box.y + vertex_2.y, 1.0,
                                    "minimap_arrow");
            }
        }

        double scale_x = get_scale_x() / Game::world_width * Game_Constants::MINIMAP_SHIP_SCALE;
        double scale_y = get_scale_y() / Game::world_height * Game_Constants::MINIMAP_SHIP_SCALE;
        Sprite player_sprite;

        player_sprite.set_name("minimap_player");

        player_sprite.render(box.x + player_x - player.get_box().w * scale_x / 2.0,
                             box.y + player_y - player.get_box().h * scale_y / 2.0, 1.0, scale_x, scale_y,
                             player.get_angle());
    }
}
