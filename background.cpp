/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "background.h"
#include "game_constants.h"
#include "game_options.h"

#include <pixels.h>
#include <engine.h>
#include <engine_math.h>
#include <object_manager.h>
#include <render.h>
#include <game_window.h>

#include <cstdint>

using namespace std;

Background_Layer::Background_Layer (double get_width, double get_height, double get_parallax_speed_x,
                                    double get_parallax_speed_y) {
    width = get_width;
    height = get_height;

    parallax_speed_x = get_parallax_speed_x;
    parallax_speed_y = get_parallax_speed_y;

    position_x_a = 0.0;
    position_x_b = 0.0;
    position_y_a = 0.0;
    position_y_b = 0.0;
}

void Background_Layer::generate_texture_background (string type, int layer_number, RNG& rng) {
    layer_image.w = width;
    layer_image.h = height;

    uint32_tLOL rmask = 0;
    uint32_t gmask = 0;
    uint32_t bmask = 0;
    uint32_t amask = 0;

    Engine::get_rgba_masks(&rmask, &gmask, &bmask, &amask);

    SDL_Surface* surface = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, rmask, gmask, bmask, amask);

    if (SDL_MUSTLOCK(surface)) {
        SDL_LockSurface(surface);
    }

    Color pixel_color;

    // Set the background color
    pixel_color.set(0, 0, 0, 0);

    // Apply the background color
    for (int x = 0; x < surface->w; x++) {
        for (int y = 0; y < surface->h; y++) {
            Pixels::surface_put_pixel(surface, x, y, pixel_color);
        }
    }

    if (type == "stars") {
        // Create the stars:

        int32_t max_attempts = Game_Constants::BASE_BACKGROUND_STARS - layer_number * 10;

        if (max_attempts < 0) {
            max_attempts = 0;
        }

        for (int32_t attempts = 0; attempts < max_attempts; attempts++) {
            // Set the star color
            string color = "";
            uint32_t random = rng.random_range(0, 99);

            if (random >= 0 && random < 1) {
                color = "star_blue";
            } else if (random >= 1 && random < 2) {
                color = "star_white";
            } else if (random >= 2 && random < 13) {
                color = "star_yellow";
            } else if (random >= 13 && random < 25) {
                color = "star_orange";
            } else {
                color = "star_red";
            }

            Color* color_ptr = Object_Manager::get_color(color);

            pixel_color.set(color_ptr->get_red(), color_ptr->get_green(), color_ptr->get_blue(), (short) 255);

            uint32_t star_size = rng.weighted_random_range(1, layer_number + 1, layer_number / 8 + 1);

            if (star_size > Game_Constants::MAX_BACKGROUND_STAR_SIZE) {
                star_size = Game_Constants::MAX_BACKGROUND_STAR_SIZE;
            }

            uint32_t x = rng.random_range(1, surface->w - 1 - star_size);
            uint32_t y = rng.random_range(1, surface->h - 1 - star_size);

            for (uint32_t int_x = x; int_x < x + star_size; int_x++) {
                for (uint32_t int_y = y; int_y < y + star_size; int_y++) {
                    Pixels::surface_put_pixel(surface, int_x, int_y, pixel_color);
                }
            }
        }
    } else if (type == "planetary") {
        int32_t max_attempts = Game_Constants::BASE_BACKGROUND_DEBRIS - layer_number * 10;

        if (max_attempts < 0) {
            max_attempts = 0;
        }

        for (int32_t attempts = 0; attempts < max_attempts; attempts++) {
            string color = "";
            uint32_t random = rng.random_range(0, 99);

            if (random < 75) {
                color = "debris_brown";
            } else if (random >= 75 && random < 90) {
                color = "debris_gray";
            } else {
                color = "debris_light_gray";
            }

            Color* color_ptr = Object_Manager::get_color(color);

            pixel_color.set(color_ptr->get_red(), color_ptr->get_green(), color_ptr->get_blue(), (short) 255);

            uint32_t object_size = rng.random_range(1, layer_number + 3);

            if (object_size > Game_Constants::MAX_BACKGROUND_DEBRIS_SIZE) {
                object_size = Game_Constants::MAX_BACKGROUND_DEBRIS_SIZE;
            }

            uint32_t x = rng.random_range(1, surface->w - 1 - object_size);
            uint32_t y = rng.random_range(1, surface->h - 1 - object_size);

            for (uint32_t int_x = x; int_x < x + object_size; int_x++) {
                for (uint32_t int_y = y; int_y < y + object_size; int_y++) {
                    Pixels::surface_put_pixel(surface, int_x, int_y, pixel_color);
                }
            }
        }
    }

    if (SDL_MUSTLOCK(surface)) {
        SDL_UnlockSurface(surface);
    }

    layer_image.load_image(surface);

    SDL_FreeSurface(surface);
}

void Background_Layer::update (double camera_delta_x, double camera_delta_y) {
    // Update the x position

    position_x_a -= camera_delta_x / parallax_speed_x;

    if (position_x_a + width < 0) {
        position_x_a = 0;
    } else if (position_x_a > 0) {
        position_x_a = -width;
    }

    position_x_b = position_x_a + width;

    // Update the y position

    position_y_a -= camera_delta_y / parallax_speed_y;

    if (position_y_a + height < 0) {
        position_y_a = 0;
    } else if (position_y_a > 0) {
        position_y_a = -height;
    }

    position_y_b = position_y_a + height;
}

void Background_Layer::render (double opacity) {
    Render::render_texture(position_x_a, position_y_a, &layer_image, opacity);
    Render::render_texture(position_x_b, position_y_a, &layer_image, opacity);
    Render::render_texture(position_x_a, position_y_b, &layer_image, opacity);
    Render::render_texture(position_x_b, position_y_b, &layer_image, opacity);
}

Background::Background () {}

void Background::setup (string type, RNG& rng) {
    uint32_t layers = 0;

    if (type == "stars") {
        layers = Game_Options::background_layers_stars;
    } else if (type == "planetary") {
        layers = Game_Options::background_layers_planetary;
    }

    for (uint32_t i = 0; i < layers; i++) {
        background_layers.push_back(Background_Layer(Game_Window::width(), Game_Window::height(),
                                                     2.0 + 2.0 * (double) i, 2.0 + 2.0 * (double) i));
    }

    for (size_t i = 0; i < background_layers.size(); i++) {
        background_layers[i].generate_texture_background(type, i, rng);
    }
}

void Background::unload () {
    for (size_t i = 0; i < background_layers.size(); i++) {
        background_layers[i].layer_image.unload_image();
    }

    background_layers.clear();
}

void Background::update (double camera_delta_x, double camera_delta_y) {
    for (size_t i = 0; i < background_layers.size(); i++) {
        background_layers[i].update(camera_delta_x, camera_delta_y);
    }
}

void Background::render (double opacity) {
    for (size_t i = 0; i < background_layers.size(); i++) {
        background_layers[i].render(opacity);
    }
}
