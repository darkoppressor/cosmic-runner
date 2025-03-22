/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "title_tree.h"
#include "game_constants.h"

using namespace std;

Title_Tree::Title_Tree (RNG& rng, const Coords<double>& position, double new_distance_scale) {
    box.x = position.x;
    box.y = position.y;

    sprite.set_name("title/title_tree");
    sprite_animation_speed = rng.random_range(Game_Constants::TITLE_TREE_ANIMATION_SPEED_MINIMUM,
                                              Game_Constants::TITLE_TREE_ANIMATION_SPEED_MAXIMUM);

    box.w = sprite.get_width();
    box.h = sprite.get_height();

    distance_scale = new_distance_scale;
}

void Title_Tree::animate () {
    sprite.animate(sprite_animation_speed);
}

void Title_Tree::render () {
    sprite.render(box.x, box.y, 1.0, distance_scale, distance_scale);
}
