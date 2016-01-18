/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef game_data_h
#define game_data_h

#include "ship_type.h"
#include "debris_type.h"

#include <progress_bar.h>
#include <file_io.h>

#include <vector>

class Game_Data{
private:

    static std::vector<Ship_Type> ship_types;
    static std::vector<Debris_Type> debris_types;

public:

    //The total number of progress bar items in load_data_game()
    static const int game_data_load_item_count;

    static void load_data_game(Progress_Bar& bar);
    //Load any data of the passed tag type
    //Returns false if passed tag type's data could not be loaded
    //Returns true otherwise
    static void load_data_tag_game(std::string tag,File_IO_Load* load);
    static void unload_data_game();

    static void load_ship_type(File_IO_Load* load);
    static Ship_Type* get_ship_type(std::string name);

    static void load_debris_type(File_IO_Load* load);
    static Debris_Type* get_debris_type(std::string name);
};

#endif
