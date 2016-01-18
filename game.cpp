/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game.h"
#include "game_data.h"
#include "background.h"

#include <render.h>
#include <game_window.h>
#include <sound_manager.h>
#include <log.h>
#include <engine.h>
#include <game_manager.h>
#include <object_manager.h>
#include <engine_data.h>
#include <engine_strings.h>

#include <ctime>

using namespace std;

vector<Ship> Game::ships;
vector<Debris> Game::debris;
vector<Effect> Game::effects;

RNG Game::rng;

double Game::world_width=0.0;
double Game::world_height=0.0;

uint64_t Game::score=0;
uint64_t Game::score_multiplier=0;

int32_t Game::contract=0;

Quadtree<double,uint32_t> Game::quadtree_debris;

Ship& Game::get_player(){
    if(!ships.empty()){
        return ships.front();
    }
    else{
        Log::add_error("Error accessing player ship");

        Engine::quit();
    }
}

void Game::clear_world(){
    Background::unload();

    ships.clear();
    debris.clear();
    effects.clear();

    world_width=0.0;
    world_height=0.0;

    score=0;
    score_multiplier=1;

    contract=-1;

    quadtree_debris.clear_tree();
}

void Game::generate_world(){
    clear_world();

    rng.seed((uint32_t)time(0));

    Background::setup();

    world_width=10000.0;
    world_height=10000.0;

    ///Generate star
    ///Generate planets

    uint32_t asteroid_count=((world_width+world_height)/2.0)/30.0;
    for(uint32_t i=0;i<asteroid_count;i++){
        string debris_type="asteroid_"+Strings::num_to_string(rng.random_range(0,0));
        Sprite debris_sprite;
        debris_sprite.set_name(Game_Data::get_debris_type(debris_type)->sprite);

        uint32_t x=rng.random_range(0,(uint32_t)world_width-(uint32_t)debris_sprite.get_width());
        uint32_t y=rng.random_range(0,(uint32_t)world_height-(uint32_t)debris_sprite.get_height());

        debris.push_back(Debris(debris_type,Coords<double>((double)x,(double)y),rng.random_range(0,359),Vector(0.01*rng.random_range(0,100),rng.random_range(0,359))));
    }

    ///Generate items

    string ship_type="player";
    Sprite ship_sprite;
    ship_sprite.set_name(Game_Data::get_ship_type(ship_type)->sprite);
    uint32_t x=rng.random_range(0,(uint32_t)world_width-(uint32_t)ship_sprite.get_width());
    uint32_t y=rng.random_range(0,(uint32_t)world_height-(uint32_t)ship_sprite.get_height());
    ships.push_back(Ship(ship_type,Coords<double>((double)x,(double)y),rng.random_range(0,359)));

    ///Make sure the player is not inside any objects

    ///Setup initial contract

    quadtree_debris.setup(10,5,0,Collision_Rect<double>(0,0,world_width,world_height));
}

uint32_t Game::get_effect_count(){
    return effects.size();
}

const Ship& Game::get_player_const(){
    if(!ships.empty()){
        return ships.front();
    }
    else{
        Log::add_error("Error accessing player ship");

        Engine::quit();
    }
}

const Debris& Game::get_debris(uint32_t index){
    if(index<debris.size()){
        return debris[index];
    }
    else{
        Log::add_error("Error accessing debris '"+Strings::num_to_string(index)+"'");

        Engine::quit();
    }
}

void Game::create_effect(string sprite,double scale,const Coords<double>& position,string sound){
    if(Game_Manager::effect_allowed()){
        effects.push_back(Effect(sprite,scale,position,sound));
        effects.back().play_sound();
    }
}

void Game::player_thrust(string direction){
    get_player().set_thrust_angle(direction);
}

void Game::player_brake(bool brake){
    get_player().set_braking(brake);
}

void Game::tick(){
}

void Game::ai(){
}

void Game::movement(){
    quadtree_debris.clear_tree();
    for(size_t i=0;i<debris.size();i++){
        quadtree_debris.insert_object(debris[i].get_collision_box(),(uint32_t)i);
    }

    for(size_t i=0;i<debris.size();i++){
        debris[i].rotation();
    }

    for(size_t i=0;i<ships.size();i++){
        ships[i].accelerate();
    }

    for(size_t i=0;i<ships.size();i++){
        ships[i].movement(quadtree_debris,rng);
    }
}

void Game::events(){
    const Ship& player=get_player_const();

    Sound_Manager::set_listener(player.get_box().center_x(),player.get_box().center_y(),Game_Manager::camera_zoom);

    for(size_t i=0;i<effects.size();){
        if(effects[i].is_done()){
            effects.erase(effects.begin()+i);
        }
        else{
            i++;
        }
    }
}

void Game::animate(){
    for(size_t i=0;i<debris.size();i++){
        debris[i].animate();
    }

    for(size_t i=0;i<ships.size();i++){
        ships[i].animate();
    }

    for(size_t i=0;i<effects.size();i++){
        effects[i].animate();
    }
}

void Game::render(){
    for(size_t i=0;i<debris.size();i++){
        debris[i].render();
    }

    for(size_t i=0;i<ships.size();i++){
        ships[i].render();
    }

    for(size_t i=0;i<effects.size();i++){
        effects[i].render();
    }
}

void Game::render_to_textures(){
    /**Rtt_Manager::set_render_target("example");
    ///Render something here
    Rtt_Manager::reset_render_target();*/
}

void Game::update_background(){
    Background::update(Game_Manager::camera_delta_x,Game_Manager::camera_delta_y);
}

void Game::render_background(){
    Render::render_rectangle(0,0,Game_Window::width(),Game_Window::height(),1.0,"ui_black");

    Background::render();
}

bool Game::move_input_state(string direction){
    if(Object_Manager::game_command_state(direction)){
        return true;
    }

    if(direction=="up" && Object_Manager::game_command_state("move_stick_vertical")<-Engine_Data::controller_dead_zone){
        return true;
    }
    else if(direction=="down" && Object_Manager::game_command_state("move_stick_vertical")>Engine_Data::controller_dead_zone){
        return true;
    }
    else if(direction=="left" && Object_Manager::game_command_state("move_stick_horizontal")<-Engine_Data::controller_dead_zone){
        return true;
    }
    else if(direction=="right" && Object_Manager::game_command_state("move_stick_horizontal")>Engine_Data::controller_dead_zone){
        return true;
    }

    return false;
}
