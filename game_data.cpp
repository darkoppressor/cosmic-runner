/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game_data.h"

#include <data_manager.h>
#include <data_reader.h>
#include <engine_strings.h>
#include <log.h>

#include <boost/algorithm/string.hpp>

using namespace std;

vector<Ship_Type> Game_Data::ship_types;
vector<Debris_Type> Game_Data::debris_types;
vector<Shot_Type> Game_Data::shot_types;
vector<Item_Type> Game_Data::item_types;
vector<Upgrade> Game_Data::upgrades;

///Don't forget to increment this for each progress item in load_data_game() below
const int Game_Data::game_data_load_item_count=5;

void Game_Data::load_data_game(Progress_Bar& bar){
    bar.progress("Loading ship types");
    Data_Manager::load_data("ship");

    bar.progress("Loading debris types");
    Data_Manager::load_data("debris");

    bar.progress("Loading shot types");
    Data_Manager::load_data("shot");

    bar.progress("Loading item types");
    Data_Manager::load_data("item");

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
    else if(tag=="item"){
        load_item_type(load);
    }
    else if(tag=="upgrade"){
        load_upgrade_type(load);
    }
}

void Game_Data::unload_data_game(){
    ship_types.clear();
    debris_types.clear();
    shot_types.clear();
    item_types.clear();
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
        else if(Data_Reader::check_prefix(line,"faction:")){
            ship_types.back().faction=line;
        }
        else if(Data_Reader::check_prefix(line,"sprite:")){
            ship_types.back().sprite=line;
        }
        else if(Data_Reader::check_prefix(line,"collision_percentage:")){
            ship_types.back().collision_percentage=Strings::string_to_double(line);
        }
        else if(Data_Reader::check_prefix(line,"upgrades:")){
            vector<string> upgrade_strings;
            boost::algorithm::split(upgrade_strings,line,boost::algorithm::is_any_of(","));

            for(size_t j=0;j<upgrade_strings.size();j++){
                if(upgrade_strings[j].length()>0){
                    ship_types.back().upgrades.push_back(upgrade_strings[j]);
                }
            }
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
        else if(Data_Reader::check_prefix(line,"item_drop_min:")){
            ship_types.back().item_drop_min=Strings::string_to_unsigned_long(line);
        }
        else if(Data_Reader::check_prefix(line,"item_drop_max:")){
            ship_types.back().item_drop_max=Strings::string_to_unsigned_long(line);
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
        else if(Data_Reader::check_prefix(line,"death_sprite:")){
            shot_types.back().death_sprite=line;
        }
        else if(Data_Reader::check_prefix(line,"death_sound:")){
            shot_types.back().death_sound=line;
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
        else if(Data_Reader::check_prefix(line,"homing:")){
            shot_types.back().homing=Strings::string_to_bool(line);
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

void Game_Data::load_item_type(File_IO_Load* load){
    item_types.push_back(Item_Type());

    vector<string> lines=Data_Reader::read_data(load,"</item>");

    for(size_t i=0;i<lines.size();i++){
        string& line=lines[i];

        if(Data_Reader::check_prefix(line,"name:")){
            item_types.back().name=line;
        }
        else if(Data_Reader::check_prefix(line,"sprite:")){
            item_types.back().sprite=line;
        }
        else if(Data_Reader::check_prefix(line,"collect_sound:")){
            item_types.back().collect_sound=line;
        }
        else if(Data_Reader::check_prefix(line,"mass:")){
            item_types.back().mass=Strings::string_to_double(line);
        }
        else if(Data_Reader::check_prefix(line,"thrust_decel:")){
            item_types.back().thrust_decel=Strings::string_to_double(line);
        }
        else if(Data_Reader::check_prefix(line,"point_value:")){
            item_types.back().point_value=Strings::string_to_unsigned_long(line);
        }
    }
}

Item_Type* Game_Data::get_item_type(string name){
    Item_Type* ptr_object=0;

    for(size_t i=0;i<item_types.size();i++){
        if(item_types[i].name==name){
            ptr_object=&item_types[i];

            break;
        }
    }

    if(ptr_object==0){
        Log::add_error("Error accessing item type '"+name+"'");
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
        else if(Data_Reader::check_prefix(line,"max_shields:")){
            upgrades.back().max_shields=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"shield_recharge_rate_up:")){
            upgrades.back().shield_recharge_rate_up=Strings::string_to_unsigned_long(line);
        }
        else if(Data_Reader::check_prefix(line,"shield_recharge_rate_down:")){
            upgrades.back().shield_recharge_rate_down=Strings::string_to_unsigned_long(line);
        }
        else if(Data_Reader::check_prefix(line,"max_hull:")){
            upgrades.back().max_hull=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"thrust_decel:")){
            upgrades.back().thrust_decel=Strings::string_to_double(line);
        }
        else if(Data_Reader::check_prefix(line,"cooldown_up:")){
            upgrades.back().cooldown_up=Strings::string_to_unsigned_long(line);
        }
        else if(Data_Reader::check_prefix(line,"cooldown_down:")){
            upgrades.back().cooldown_down=Strings::string_to_unsigned_long(line);
        }
        else if(Data_Reader::check_prefix(line,"solid_damage_mod:")){
            upgrades.back().solid_damage_mod=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"explosive_damage_mod:")){
            upgrades.back().explosive_damage_mod=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"energy_damage_mod:")){
            upgrades.back().energy_damage_mod=Strings::string_to_long(line);
        }
        else if(Data_Reader::check_prefix(line,"anti_gravity:")){
            upgrades.back().anti_gravity=Strings::string_to_bool(line);
        }
        else if(Data_Reader::check_prefix(line,"point_defense:")){
            upgrades.back().point_defense=Strings::string_to_bool(line);
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
