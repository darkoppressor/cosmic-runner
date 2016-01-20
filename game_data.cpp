/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game_data.h"

#include <data_manager.h>
#include <data_reader.h>
#include <engine_strings.h>
#include <log.h>

using namespace std;

vector<Ship_Type> Game_Data::ship_types;
vector<Debris_Type> Game_Data::debris_types;
vector<Shot_Type> Game_Data::shot_types;
vector<Upgrade> Game_Data::upgrades;

///Don't forget to increment this for each progress item in load_data_game() below
const int Game_Data::game_data_load_item_count=4;

void Game_Data::load_data_game(Progress_Bar& bar){
    bar.progress("Loading ship types");
    Data_Manager::load_data("ship");

    bar.progress("Loading debris types");
    Data_Manager::load_data("debris");

    bar.progress("Loading shot types");
    Data_Manager::load_data("shot");

    bar.progress("Loading upgrade types");
    Data_Manager::load_data("upgrade");
}

void Game_Data::load_data_tag_game(string tag,File_IO_Load* load){
    if(tag=="ship"){
        load_ship_type(load);
    }
    else if(tag=="debris"){
        load_debris_type(load);
    }
    else if(tag=="shot"){
        load_shot_type(load);
    }
    else if(tag=="upgrade"){
        load_upgrade_type(load);
    }
}

void Game_Data::unload_data_game(){
    ship_types.clear();
    debris_types.clear();
    shot_types.clear();
    upgrades.clear();
}

void Game_Data::load_ship_type(File_IO_Load* load){
    ship_types.push_back(Ship_Type());

    vector<string> lines=Data_Reader::read_data(load,"</ship>");

    for(size_t i=0;i<lines.size();i++){
        string& line=lines[i];

        if(Data_Reader::check_prefix(line,"name:")){
            ship_types.back().name=line;
        }
        else if(Data_Reader::check_prefix(line,"sprite:")){
            ship_types.back().sprite=line;
        }
        else if(Data_Reader::check_prefix(line,"collision_percentage:")){
            ship_types.back().collision_percentage=Strings::string_to_double(line);
        }
        else if(Data_Reader::check_prefix(line,"mass:")){
            ship_types.back().mass=Strings::string_to_double(line);
        }
        else if(Data_Reader::check_prefix(line,"thrust_accel:")){
            ship_types.back().thrust_accel=Strings::string_to_double(line);
        }
        else if(Data_Reader::check_prefix(line,"thrust_decel:")){
            ship_types.back().thrust_decel=Strings::string_to_double(line);
        }
        else if(Data_Reader::check_prefix(line,"max_speed:")){
            ship_types.back().max_speed=Strings::string_to_double(line);
        }
        else if(Data_Reader::check_prefix(line,"hull_max:")){
            ship_types.back().hull_max=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"shields_max:")){
            ship_types.back().shields_max=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"point_value:")){
            ship_types.back().point_value=Strings::string_to_unsigned_long(line);
        }
    }
}

Ship_Type* Game_Data::get_ship_type(string name){
    Ship_Type* ptr_object=0;

    for(size_t i=0;i<ship_types.size();i++){
        if(ship_types[i].name==name){
            ptr_object=&ship_types[i];

            break;
        }
    }

    if(ptr_object==0){
        Log::add_error("Error accessing ship type '"+name+"'");
    }

    return ptr_object;
}

void Game_Data::load_debris_type(File_IO_Load* load){
    debris_types.push_back(Debris_Type());

    vector<string> lines=Data_Reader::read_data(load,"</debris>");

    for(size_t i=0;i<lines.size();i++){
        string& line=lines[i];

        if(Data_Reader::check_prefix(line,"name:")){
            debris_types.back().name=line;
        }
        else if(Data_Reader::check_prefix(line,"sprite:")){
            debris_types.back().sprite=line;
        }
        else if(Data_Reader::check_prefix(line,"collision_percentage:")){
            debris_types.back().collision_percentage=Strings::string_to_double(line);
        }
        else if(Data_Reader::check_prefix(line,"damage:")){
            debris_types.back().damage=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"damage_type:")){
            debris_types.back().damage_type=line;
        }
    }
}

Debris_Type* Game_Data::get_debris_type(string name){
    Debris_Type* ptr_object=0;

    for(size_t i=0;i<debris_types.size();i++){
        if(debris_types[i].name==name){
            ptr_object=&debris_types[i];

            break;
        }
    }

    if(ptr_object==0){
        Log::add_error("Error accessing debris type '"+name+"'");
    }

    return ptr_object;
}

void Game_Data::load_shot_type(File_IO_Load* load){
    shot_types.push_back(Shot_Type());

    vector<string> lines=Data_Reader::read_data(load,"</shot>");

    for(size_t i=0;i<lines.size();i++){
        string& line=lines[i];

        if(Data_Reader::check_prefix(line,"name:")){
            shot_types.back().name=line;
        }
        else if(Data_Reader::check_prefix(line,"sprite:")){
            shot_types.back().sprite=line;
        }
        else if(Data_Reader::check_prefix(line,"collision_percentage:")){
            shot_types.back().collision_percentage=Strings::string_to_double(line);
        }
        else if(Data_Reader::check_prefix(line,"mass:")){
            shot_types.back().mass=Strings::string_to_double(line);
        }
        else if(Data_Reader::check_prefix(line,"damage_type:")){
            shot_types.back().damage_type=line;
        }
    }
}

Shot_Type* Game_Data::get_shot_type(string name){
    Shot_Type* ptr_object=0;

    for(size_t i=0;i<shot_types.size();i++){
        if(shot_types[i].name==name){
            ptr_object=&shot_types[i];

            break;
        }
    }

    if(ptr_object==0){
        Log::add_error("Error accessing shot type '"+name+"'");
    }

    return ptr_object;
}

void Game_Data::load_upgrade_type(File_IO_Load* load){
    upgrades.push_back(Upgrade());

    vector<string> lines=Data_Reader::read_data(load,"</upgrade>");

    for(size_t i=0;i<lines.size();i++){
        string& line=lines[i];

        if(Data_Reader::check_prefix(line,"name:")){
            upgrades.back().name=line;
        }
        else if(Data_Reader::check_prefix(line,"display_name:")){
            upgrades.back().display_name=line;
        }
        else if(Data_Reader::check_prefix(line,"type:")){
            upgrades.back().type=line;
        }
        else if(Data_Reader::check_prefix(line,"sound:")){
            upgrades.back().sound=line;
        }
        else if(Data_Reader::check_prefix(line,"cooldown:")){
            upgrades.back().cooldown=Strings::string_to_unsigned_long(line);
        }
        else if(Data_Reader::check_prefix(line,"shot_type:")){
            upgrades.back().shot_type=line;
        }
        else if(Data_Reader::check_prefix(line,"spread_type:")){
            upgrades.back().spread_type=line;
        }
        else if(Data_Reader::check_prefix(line,"thrust_accel:")){
            upgrades.back().thrust_accel=Strings::string_to_double(line);
        }
        else if(Data_Reader::check_prefix(line,"max_speed:")){
            upgrades.back().max_speed=Strings::string_to_double(line);
        }
        else if(Data_Reader::check_prefix(line,"damage:")){
            upgrades.back().damage=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"shots:")){
            upgrades.back().shots=Strings::string_to_unsigned_long(line);
        }
        else if(Data_Reader::check_prefix(line,"range:")){
            upgrades.back().range=Strings::string_to_double(line);
        }
    }
}

Upgrade* Game_Data::get_upgrade_type(string name){
    Upgrade* ptr_object=0;

    for(size_t i=0;i<upgrades.size();i++){
        if(upgrades[i].name==name){
            ptr_object=&upgrades[i];

            break;
        }
    }

    if(ptr_object==0){
        Log::add_error("Error accessing upgrade '"+name+"'");
    }

    return ptr_object;
}

vector<string> Game_Data::get_upgrade_names(){
    vector<string> names;

    for(size_t i=0;i<upgrades.size();i++){
        names.push_back(upgrades[i].name);
    }

    return names;
}
