/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game.h"
#include "game_data.h"
#include "background.h"
#include "game_constants.h"

#include <render.h>
#include <game_window.h>
#include <sound_manager.h>
#include <log.h>
#include <engine.h>
#include <game_manager.h>
#include <object_manager.h>
#include <engine_data.h>
#include <engine_strings.h>
#include <window_manager.h>

#include <ctime>

using namespace std;

vector<Ship> Game::ships;
vector<Debris> Game::debris;
vector<Effect> Game::effects;
vector<Planet> Game::planets;
vector<Shot> Game::shots;

int32_t Game::contract=0;

Sprite Game::contract_sprite;
Sprite Game::no_contract_sprite;

int32_t Game::landed_planet=0;

uint64_t Game::score=0;
uint64_t Game::score_multiplier=0;

vector<string> Game::upgrade_list;

Quadtree<double,uint32_t> Game::quadtree_debris;
Quadtree<double,uint32_t> Game::quadtree_shots;

RNG Game::rng;

uint32_t Game::frame=0;

double Game::world_width=0.0;
double Game::world_height=0.0;

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
    planets.clear();
    shots.clear();

    world_width=0.0;
    world_height=0.0;

    score=0;
    score_multiplier=1;

    upgrade_list.clear();

    contract=-1;

    contract_sprite.reset_animation();
    no_contract_sprite.reset_animation();

    landed_planet=-1;

    quadtree_debris.clear_tree();
    quadtree_shots.clear_tree();

    frame=0;
}

void Game::generate_world(){
    clear_world();

    contract_sprite.set_name("contract_indicator_arrow");
    no_contract_sprite.set_name("contract_indicator_none");

    rng.seed((uint32_t)time(0));

    Background::setup();

    world_width=10000.0;
    world_height=10000.0;

    ///QQQ Generate star

    //Generate planets
    uint32_t planet_count=((world_width+world_height)/2.0)/2048.0;
    if(planet_count<2){
        planet_count=2;
    }
    for(uint32_t i=0;i<planet_count;i++){
        Sprite planet_sprite;
        planet_sprite.set_name("planet_"+Strings::num_to_string(rng.random_range(0,0)));

        uint32_t radius=uint32_t(planet_sprite.get_width()/2.0);

        uint32_t x=rng.random_range(radius,(uint32_t)world_width-radius);
        uint32_t y=rng.random_range(radius,(uint32_t)world_height-radius);

        planets.push_back(Planet(planet_sprite.name,Coords<double>((double)x,(double)y)));
    }

    //Generate debris
    uint32_t asteroid_count=((world_width+world_height)/2.0)/24.0;
    for(uint32_t i=0;i<asteroid_count;i++){
        string debris_type="asteroid_"+Strings::num_to_string(rng.random_range(0,0));
        Sprite debris_sprite;
        debris_sprite.set_name(Game_Data::get_debris_type(debris_type)->sprite);

        uint32_t x=rng.random_range(0,(uint32_t)world_width-(uint32_t)debris_sprite.get_width());
        uint32_t y=rng.random_range(0,(uint32_t)world_height-(uint32_t)debris_sprite.get_height());

        debris.push_back(Debris(debris_type,Coords<double>((double)x,(double)y),rng.random_range(0,359),Vector(0.01*rng.random_range(0,100),rng.random_range(0,359))));
    }

    ///QQQ Generate items

    ///QQQ Try to get number of planets desired, but don't allow any too near the star, and don't allow any to touch each other
    ///QQQ If debris is too near the star or touching other debris, skip it
    ///QQQ If item is too near the star or touching a planet, or touching another item, skip it

    //Generate the player's ship
    string ship_type="player";
    Sprite ship_sprite;
    ship_sprite.set_name(Game_Data::get_ship_type(ship_type)->sprite);
    uint32_t random_planet_index=rng.random_range(0,planets.size()-1);
    const Planet& planet=planets[random_planet_index];
    ships.push_back(Ship(ship_type,Coords<double>(planet.get_circle().x-ship_sprite.get_width()/2.0,planet.get_circle().y-ship_sprite.get_height()/2.0),rng.random_range(0,359)));

    //Setup initial contract
    assign_new_contract(get_nearest_planet());

    quadtree_debris.setup(10,5,0,Collision_Rect<double>(0,0,world_width,world_height));
    quadtree_shots.setup(10,5,0,Collision_Rect<double>(0,0,world_width,world_height));
}

uint32_t Game::get_ship_count(){
    return ships.size();
}

uint32_t Game::get_debris_count(){
    return debris.size();
}

uint32_t Game::get_effect_count(){
    return effects.size();
}

uint32_t Game::get_planet_count(){
    return planets.size();
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

const Shot& Game::get_shot(uint32_t index){
    if(index<shots.size()){
        return shots[index];
    }
    else{
        Log::add_error("Error accessing shot '"+Strings::num_to_string(index)+"'");

        Engine::quit();
    }
}

const Planet& Game::get_planet(uint32_t index){
    if(index<planets.size()){
        return planets[index];
    }
    else{
        Log::add_error("Error accessing planet '"+Strings::num_to_string(index)+"'");

        Engine::quit();
    }
}

uint64_t Game::get_score(){
    return score;
}

uint64_t Game::get_score_multiplier(){
    return score_multiplier;
}

vector<string> Game::get_upgrade_list(){
    return upgrade_list;
}

RNG& Game::get_rng(){
    return rng;
}

void Game::increase_score(uint64_t amount){
    uint64_t available_space=UINT64_MAX-score;

    if(available_space>=amount){
        score+=amount;
    }
    else{
        score=UINT64_MAX;
    }
}

void Game::increase_score_multiplier(uint64_t amount){
    uint64_t available_space=UINT64_MAX-score_multiplier;

    if(available_space>=amount){
        score_multiplier+=amount;
    }
    else{
        score_multiplier=UINT64_MAX;
    }
}

uint32_t Game::get_nearest_planet(){
    int32_t nearest_index=-1;
    double nearest_distance=0.0;

    for(size_t i=0;i<planets.size();i++){
        const Ship& player=get_player_const();
        const Planet& planet=planets[i];

        double new_distance=Math::distance_between_points(player.get_box().center_x(),player.get_box().center_y(),planet.get_circle().x,planet.get_circle().y);

        if(nearest_index==-1 || new_distance<nearest_distance){
            nearest_index=i;
            nearest_distance=new_distance;
        }
    }

    return (uint32_t)nearest_index;
}

void Game::assign_new_contract(uint32_t current_planet){
    uint32_t index=0;

    do{
        index=rng.random_range(0,planets.size()-1);
    }while(index==current_planet);

    contract=index;
}

bool Game::player_has_contract(){
    return contract>=0;
}

const Planet& Game::get_contract_planet(){
    if(player_has_contract()){
        return planets[contract];
    }
    else{
        return planets[0];
    }
}

uint32_t Game::get_contract_planet_index(){
    if(player_has_contract()){
        return contract;
    }
    else{
        return 0;
    }
}

void Game::complete_contract(){
    increase_score_multiplier(Game_Constants::SCORE_MULTIPLIER_INCREASE);

    uint32_t current_planet=contract;

    assign_new_contract(current_planet);

    landed_planet=-1;

    Game_Manager::paused=false;

    Engine::make_toast("Contract completed\nScore multiplier +"+Strings::num_to_string(Game_Constants::SCORE_MULTIPLIER_INCREASE));
}

void Game::cancel_contract(){
    contract=-1;

    Engine::make_toast("Contract abandoned");
}

void Game::build_upgrade_list(){
    ///QQQ make this actually work

    upgrade_list.clear();

    for(uint32_t i=0;i<Game_Constants::UPGRADE_LIST_SIZE;i++){
        upgrade_list.push_back("placeholder");
    }
}

void Game::commence_landing(uint32_t landing_planet_index){
    get_player().commence_landing(landing_planet_index);
}

void Game::land(uint32_t landing_planet_index){
    Game_Manager::paused=true;

    landed_planet=landing_planet_index;

    if(player_has_contract() && landed_planet==contract){
        build_upgrade_list();

        Window_Manager::get_window("select_upgrade")->toggle_on(true,true);
    }
    else if(!player_has_contract()){
        assign_new_contract((uint32_t)landed_planet);

        landed_planet=-1;

        Game_Manager::paused=false;

        Engine::make_toast("New contract acquired");
    }
}

bool Game::player_is_landing(){
    return get_player().is_landing();
}

bool Game::player_is_landed(){
    return landed_planet>=0;
}

const Planet& Game::get_landed_planet(){
    if(player_is_landed()){
        return planets[landed_planet];
    }
    else{
        return planets[0];
    }
}

void Game::create_effect(string sprite,double scale,const Coords<double>& position,string sound,const Vector& velocity,double angle,
                         const Vector& angular_velocity,uint32_t seconds){
    if(Game_Manager::effect_allowed()){
        effects.push_back(Effect(sprite,scale,position,sound,velocity,angle,angular_velocity,seconds));
        effects.back().play_sound();
    }
}

void Game::player_thrust(string direction){
    get_player().set_thrust_angle(direction);
}

void Game::player_brake(bool brake){
    get_player().set_braking(brake);
}

void Game::kill_shot(uint32_t index){
    if(index<shots.size()){
        shots[index].die();
    }
}

void Game::game_over(){
    Game_Manager::paused=true;

    Window_Manager::get_window("game_over")->toggle_on(true,true);
}

void Game::tick(){
    if(++frame>=Engine::UPDATE_RATE){
        frame=0;
    }
}

void Game::ai(){
}

void Game::movement(){
    quadtree_debris.clear_tree();
    for(size_t i=0;i<debris.size();i++){
        quadtree_debris.insert_object(debris[i].get_collision_box(),(uint32_t)i);
    }

    quadtree_shots.clear_tree();
    for(size_t i=0;i<shots.size();i++){
        quadtree_shots.insert_object(shots[i].get_collision_box(),(uint32_t)i);
    }

    for(size_t i=0;i<debris.size();i++){
        debris[i].rotation();
    }

    for(size_t i=0;i<ships.size();i++){
        ships[i].land(i==0);

        ships[i].regenerate_shields();

        ships[i].accelerate(i==0,frame);
    }

    for(size_t i=0;i<ships.size();i++){
        ships[i].movement(i==0,quadtree_debris,quadtree_shots,rng);
    }

    for(size_t i=0;i<shots.size();i++){
        shots[i].accelerate();
    }

    for(size_t i=0;i<shots.size();i++){
        shots[i].movement(quadtree_debris);
    }

    for(size_t i=0;i<effects.size();i++){
        effects[i].countdown();

        effects[i].movement();
    }
}

void Game::events(){
    const Ship& player=get_player_const();

    Sound_Manager::set_listener(player.get_box().center_x(),player.get_box().center_y(),Game_Manager::camera_zoom);

    for(size_t i=1;i<ships.size();){
        if(!ships[i].is_alive()){
            ships.erase(ships.begin()+i);
        }
        else{
            i++;
        }
    }

    for(size_t i=0;i<shots.size();){
        if(!shots[i].is_alive()){
            shots.erase(shots.begin()+i);
        }
        else{
            i++;
        }
    }

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
    for(size_t i=0;i<planets.size();i++){
        planets[i].animate();
    }

    for(size_t i=0;i<debris.size();i++){
        debris[i].animate();
    }

    for(size_t i=0;i<ships.size();i++){
        ships[i].animate();
    }

    for(size_t i=0;i<shots.size();i++){
        shots[i].animate();
    }

    for(size_t i=0;i<effects.size();i++){
        effects[i].animate();
    }

    if(player_has_contract()){
        contract_sprite.animate();
    }
    else{
        no_contract_sprite.animate();
    }
}

void Game::render(){
    for(size_t i=0;i<planets.size();i++){
        planets[i].render();
    }

    for(size_t i=0;i<debris.size();i++){
        debris[i].render();
    }

    for(size_t i=0;i<ships.size();i++){
        ships[i].render();
    }

    for(size_t i=0;i<shots.size();i++){
        shots[i].render();
    }

    for(size_t i=0;i<effects.size();i++){
        effects[i].render();
    }

    if(player_has_contract()){
        const Planet& planet=get_contract_planet();
        const Ship& player=get_player_const();

        Collision_Rect<double> box_contract_indicator(((Game_Window::width()-contract_sprite.get_width())/2.0)*Game_Manager::camera_zoom+Game_Manager::camera.x,
                                                      ((Game_Window::height()/4.0-contract_sprite.get_height())/2.0)*Game_Manager::camera_zoom+Game_Manager::camera.y,
                                                      contract_sprite.get_width(),contract_sprite.get_height());

        contract_sprite.render((Game_Window::width()-contract_sprite.get_width())/2.0,(Game_Window::height()/4.0-contract_sprite.get_height())/2.0,1.0,1.0,1.0,
                               Collision::get_angle_to_circ(box_contract_indicator,planet.get_circle()));
    }
    else{
        no_contract_sprite.render((Game_Window::width()-no_contract_sprite.get_width())/2.0,(Game_Window::height()/4.0-no_contract_sprite.get_height())/2.0);
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
