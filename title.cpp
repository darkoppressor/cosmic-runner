/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "title.h"
#include "game_constants.h"

#include <game_window.h>
#include <engine.h>
#include <engine_strings.h>
#include <render.h>
#include <image_manager.h>

using namespace std;

Title::Title () {
    ship_spawn_check = 0;
}

void Title::clear_title () {
    ships.clear();
    trees.clear();
    suns.clear();

    ship_spawn_check = 0;
}

void Title::setup (RNG& rng) {
    Sprite sprite;

    sprite.set_name("title_sun");

    Coords<double> position((double) Game_Window::width() / 4.0 - sprite.get_width() / 2.0,
                            (double) Game_Window::height() - sprite.get_height() * 0.9);

    suns.push_back(Title_Sun(position));
    suns.back().setup(sprite.name);

    generate_ship(rng);

    uint32_t max_trees = rng.random_range(Game_Constants::TITLE_MAX_TREES_MINIMUM,
                                          Game_Constants::TITLE_MAX_TREES_MAXIMUM);

    for (uint32_t i = 0; i < max_trees; i++) {
        double distance_scale = 0.01 * (double) rng.random_range(75, 150);
        Sprite sprite;
        sprite.set_name("title_tree");

        Coords<double> position(rng.random_range(
                                    (uint32_t) (sprite.get_width() * distance_scale),
                                    (uint32_t) ((double) Game_Window::width() - sprite.get_width() * distance_scale)), (double) rng.random_range(
                                    Game_Constants::TITLE_TREE_MAXIMUM_HEIGHT,
                                    (uint32_t) Game_Window::height()) - sprite.get_height() * distance_scale);

        trees.push_back(Title_Tree(rng, position, distance_scale));
    }
}

void Title::generate_ship (RNG& rng) {
    if (ships.size() < Game_Constants::TITLE_MAX_SHIPS) {
        bool bird = rng.random_range(0, 99) < Game_Constants::TITLE_BIRD_CHANCE;
        bool cloud = !bird ? rng.random_range(0, 99) < Game_Constants::TITLE_CLOUD_CHANCE : false;
        Sprite sprite;

        if (bird) {
            sprite.set_name("title_bird");
        } else if (cloud) {
            sprite.set_name("title_cloud_" + Strings::num_to_string(rng.random_range(0, 2)));
        } else {
            sprite.set_name("title_ship_" + Strings::num_to_string(rng.random_range(0, 2)));
        }

        double distance_scale = 0.01 * (double) rng.random_range(20, 80);

        if (bird) {
            distance_scale = 0.01 * (double) rng.random_range(75, 200);
        } else if (cloud) {
            distance_scale = 0.01 * (double) rng.random_range(50, 150);
        }

        Coords<double> position;
        Vector velocity;
        velocity.magnitude = distance_scale * 0.1 * (double) rng.random_range(200, 400);

        if (bird) {
            velocity.magnitude = distance_scale * 0.1 * (double) rng.random_range(25, 75);
        } else if (cloud) {
            velocity.magnitude = distance_scale * 0.1 * (double) rng.random_range(20, 80);
        }

        if (rng.random_range(0, 99) < 50) {
            position.x = 0.0 - sprite.get_width() * distance_scale;
            velocity.direction = 0.0;
        } else {
            position.x = (double) Game_Window::width();
            velocity.direction = 180.0;
        }

        if (bird) {
            position.y =
                rng.random_range(Game_Constants::TITLE_SHIP_MINIMUM_HEIGHT,
                                 (uint32_t) (Game_Window::height() - sprite.get_height()));
        } else if (cloud) {
            position.y = rng.random_range(0, (uint32_t) (Game_Window::height() - sprite.get_height()));
        } else {
            uint32_t minimum_height = (uint32_t) (Game_Window::height() - sprite.get_height());

            if (minimum_height > Game_Constants::TITLE_SHIP_MINIMUM_HEIGHT) {
                minimum_height = Game_Constants::TITLE_SHIP_MINIMUM_HEIGHT;
            }

            position.y = rng.random_range(0, minimum_height);
        }

        ships.push_back(Title_Ship(sprite.name, position, velocity, distance_scale));
    }
}

void Title::movement () {
    for (size_t i = 0; i < ships.size(); i++) {
        ships[i].movement();
    }
}

void Title::check_ships (RNG& rng) {
    for (size_t i = 0; i < ships.size();) {
        if (!ships[i].is_alive()) {
            ships.erase(ships.begin() + i);
        } else {
            i++;
        }
    }

    if (++ship_spawn_check >= Game_Constants::TITLE_SHIP_SPAWN_RATE* Engine::UPDATE_RATE) {
        ship_spawn_check = 0;

        generate_ship(rng);
    }
}

void Title::animate (RNG& rng) {
    for (size_t i = 0; i < suns.size(); i++) {
        suns[i].animate(rng);
    }

    for (size_t i = 0; i < ships.size(); i++) {
        ships[i].animate();
    }

    for (size_t i = 0; i < trees.size(); i++) {
        trees[i].animate();
    }
}

void Title::render () {
    Render::render_texture(0.0, 0.0, Image_Manager::get_image("title_background_0"));

    for (size_t i = 0; i < suns.size(); i++) {
        suns[i].render();
    }

    Render::render_texture(0.0, 0.0, Image_Manager::get_image("title_background_1"));

    for (size_t i = 0; i < ships.size(); i++) {
        ships[i].render(false);
    }

    Render::render_texture(0.0, 0.0, Image_Manager::get_image("title_background_2"));

    for (size_t i = 0; i < ships.size(); i++) {
        ships[i].render(true);
    }

    for (size_t i = 0; i < trees.size(); i++) {
        trees[i].render();
    }
}
