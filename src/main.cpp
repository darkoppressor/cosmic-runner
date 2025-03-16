/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "main.h"
#include "game/game_data.h"

#include <main_startup.h>

using namespace std;

int main (int argc, char* args[]) {
    return main_startup(argc, args, Game_Data::game_data_load_item_count);
}
