/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef game_data_h
#define game_data_h

#include "ship_type.h"
#include "debris_type.h"
#include "shot_type.h"
#include "item_type.h"
#include "upgrade.h"
#include "planet_type.h"

#include <ui/progress_bar.h>
#include <file/file_io.h>

#include <vector>

class Game_Data {
    private:
        static std::vector<Ship_Type> ship_types;
        static std::vector<Debris_Type> debris_types;
        static std::vector<Shot_Type> shot_types;
        static std::vector<Item_Type> item_types;
        static std::vector<Upgrade> upgrades;
        static std::vector<Planet_Type> planet_types;

    public:
        // The total number of progress bar items in load_data_game()
        static const int game_data_load_item_count;

        static void load_data_game(Progress_Bar& bar);
        // Load any data of the passed tag type
        // Returns false if passed tag type's data could not be loaded
        // Returns true otherwise
        static void load_data_tag_game(std::string tag, File_IO_Load* load);
        static void unload_data_game();
        static void load_ship_type(File_IO_Load* load);
        static Ship_Type* get_ship_type(std::string name);
        static void load_debris_type(File_IO_Load* load);
        static Debris_Type* get_debris_type(std::string name);
        static void load_shot_type(File_IO_Load* load);
        static Shot_Type* get_shot_type(std::string name);
        static void load_item_type(File_IO_Load* load);
        static Item_Type* get_item_type(std::string name);
        static void load_upgrade_type(File_IO_Load* load);
        static Upgrade* get_upgrade_type(std::string name);
        static std::vector<std::string> get_upgrade_names();
        static void load_planet_type(File_IO_Load* load);
        static Planet_Type* get_planet_type(std::string name);
};

#endif
