/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "title_sun.h"
#include "game_constants.h"

#include <render/render.h>
#include <engine.h>
#include <image/image_manager.h>
#include <render/pixels.h>

using namespace std;

Title_Sun_Shimmer::Title_Sun_Shimmer (double new_y, double new_y_right_mod, bool new_left_side, bool new_right_side,
                                      double new_speed, int32_t new_radius) {
    y = new_y;
    y_right_mod = new_y_right_mod;

    left_side = new_left_side;
    right_side = new_right_side;

    speed = new_speed;

    radius = new_radius;
}

double Title_Sun_Shimmer::get_y () const {
    return y;
}

bool Title_Sun_Shimmer::is_done (double sun_height) const {
    return (!left_side || y >= sun_height) && (!right_side || y + y_right_mod >= sun_height);
}

void Title_Sun_Shimmer::movement () {
    y += speed;
}

void Title_Sun_Shimmer::draw_circle (SDL_Surface* surface, int32_t x_coord, int32_t y_coord) const {
    for (int32_t x = x_coord - radius; x < x_coord + radius; x++) {
        for (int32_t y = y_coord - radius; y < y_coord + radius; y++) {
            if (x >= 0 && x < surface->w && y >= 0 && y < surface->h) {
                if (Collision::check_circ(Collision_Circ<double>(x, y, 0.0),
                                          Collision_Circ<double>(x_coord, y_coord, radius))) {
                    Color current_color = Pixels::surface_get_pixel(surface, x, y);

                    current_color.set(current_color.get_red(), current_color.get_green(), current_color.get_blue(),
                                      short(current_color.get_alpha() / 2));
                    Pixels::surface_put_pixel(surface, x, y, current_color);
                }
            }
        }
    }
}

void Title_Sun_Shimmer::draw_left (SDL_Surface* surface) const {
    int32_t y_coord = (int32_t) y;
    int32_t x_coord = -1;

    if (y_coord >= 0 && y_coord < surface->h) {
        for (int32_t x = 0; x < surface->w; x++) {
            if (Pixels::surface_get_pixel(surface, x, y_coord).get_alpha() > 0) {
                x_coord = x;

                break;
            }
        }
    }

    if (x_coord >= 0) {
        draw_circle(surface, x_coord, y_coord);
    }
}

void Title_Sun_Shimmer::draw_right (SDL_Surface* surface) const {
    int32_t y_coord = (int32_t) (y + y_right_mod);
    int32_t x_coord = -1;

    if (y_coord >= 0 && y_coord < surface->h) {
        for (int32_t x = surface->w - 1; x >= 0; x--) {
            if (Pixels::surface_get_pixel(surface, x, y_coord).get_alpha() > 0) {
                x_coord = x;

                break;
            }
        }
    }

    if (x_coord >= 0) {
        draw_circle(surface, x_coord, y_coord);
    }
}

void Title_Sun_Shimmer::draw (SDL_Surface* surface) const {
    if (left_side) {
        draw_left(surface);
    }

    if (right_side) {
        draw_right(surface);
    }
}

Title_Sun::Title_Sun (const Coords<double>& position) {
    box.x = position.x;
    box.y = position.y;

    base_surface = 0;

    shimmer_spawn = 0;
}

Title_Sun::~Title_Sun () {
    if (base_surface != 0) {
        SDL_FreeSurface(base_surface);

        base_surface = 0;
    }

    if (image.texture != 0) {
        image.unload_image();
    }
}

void Title_Sun::setup (string sprite_name) {
    base_surface = Render::load_image("images/" + sprite_name + ".png");

    if (base_surface != 0) {
        SDL_SetSurfaceBlendMode(base_surface, SDL_BLENDMODE_NONE);

        box.w = base_surface->w;
        box.h = base_surface->h;
    }
}

void Title_Sun::animate (RNG& rng) {
    for (size_t i = 0; i < shimmers.size();) {
        shimmers[i].movement();

        if (shimmers[i].is_done(box.h)) {
            shimmers.erase(shimmers.begin() + i);
        } else {
            i++;
        }
    }

    if (++shimmer_spawn >= Game_Constants::TITLE_SUN_SHIMMER_SPAWN_RATE* Engine::UPDATE_RATE / 1000) {
        shimmer_spawn = 0;

        if (shimmers.size() < Game_Constants::TITLE_MAX_SUN_SHIMMERS && rng.random_range(0,
                                                                                         99) <
            Game_Constants::TITLE_SUN_SHIMMER_SPAWN_CHANCE) {
            double y_right_mod = rng.random_range(Game_Constants::TITLE_SUN_SHIMMER_Y_RIGHT_MOD_MIN,
                                                  Game_Constants::TITLE_SUN_SHIMMER_Y_RIGHT_MOD_MAX);

            if (rng.random_range(0, 99) < 50) {
                y_right_mod *= -1.0;
            }

            bool left_side = true;
            bool right_side = true;
            uint32_t random_side = rng.random_range(0, 99);

            if (random_side < 20) {
                left_side = false;
            } else if (random_side < 40) {
                right_side = false;
            }

            shimmers.push_back(Title_Sun_Shimmer(0.0, y_right_mod, left_side, right_side,
                                                 0.01 *
                                                 (double) rng.random_range(Game_Constants::TITLE_SUN_SHIMMER_SPEED_MIN,
                                                                           Game_Constants::TITLE_SUN_SHIMMER_SPEED_MAX),
                                                 rng.random_range(Game_Constants::TITLE_SUN_SHIMMER_RADIUS_MIN,
                                                                  Game_Constants::TITLE_SUN_SHIMMER_RADIUS_MAX)));
        }
    }

    if (base_surface != 0) {
        if (image.texture != 0) {
            image.unload_image();
        }

        image.w = box.w;
        image.h = box.h;

        SDL_Surface* surface = SDL_CreateRGBSurface(base_surface->flags, base_surface->w, base_surface->h, 32,
                                                    base_surface->format->Rmask, base_surface->format->Gmask,
                                                    base_surface->format->Bmask, base_surface->format->Amask);

        SDL_SetSurfaceBlendMode(surface, SDL_BLENDMODE_NONE);

        SDL_BlitSurface(base_surface, 0, surface, 0);

        if (SDL_MUSTLOCK(surface)) {
            SDL_LockSurface(surface);
        }

        for (size_t i = 0; i < shimmers.size(); i++) {
            shimmers[i].draw(surface);
        }

        if (SDL_MUSTLOCK(surface)) {
            SDL_UnlockSurface(surface);
        }

        image.load_image(surface);

        SDL_FreeSurface(surface);
    }
}

void Title_Sun::render () {
    if (image.texture != 0) {
        Render::render_texture(box.x, box.y, &image);
    }
}
