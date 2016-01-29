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
vector<Star> Game::stars;
vector<Effect> Game::effects;
vector<Planet> Game::planets;
vector<Shot> Game::shots;
vector<Explosion> Game::explosions;
vector<Item> Game::items;

int32_t Game::contract=0;

Sprite Game::contract_sprite;
Sprite Game::no_contract_sprite;

int32_t Game::landed_planet=0;

uint64_t Game::score=0;
uint64_t Game::score_multiplier=0;

uint32_t Game::power=0;

uint32_t Game::notoriety=0;

bool Game::player_tractored=false;
uint32_t Game::tractoring_ship=0;
double Game::tractor_angle=0.0;
Sprite Game::tractor_sprite;

Vector Game::player_acceleration;

vector<string> Game::upgrade_list;

Quadtree<double,uint32_t> Game::quadtree_debris;
Quadtree<double,uint32_t> Game::quadtree_shots;
Quadtree<double,uint32_t> Game::quadtree_ships;
Quadtree<double,uint32_t> Game::quadtree_explosions;
Quadtree<double,uint32_t> Game::quadtree_items;
Quadtree<double,uint32_t> Game::quadtree_planets;

RNG Game::rng;

uint32_t Game::frame=0;
uint32_t Game::ship_spawn_check=0;

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
    stars.clear();
    effects.clear();
    planets.clear();
    shots.clear();
    explosions.clear();
    items.clear();

    world_width=0.0;
    world_height=0.0;

    score=0;
    score_multiplier=1;

    power=Game_Constants::MAX_POWER*Engine::UPDATE_RATE;

    notoriety=0;

    clear_tractor();
    tractor_sprite.set_name("tractor_beam");

    player_acceleration*=0.0;

    upgrade_list.clear();

    contract=-1;

    contract_sprite.reset_animation();
    no_contract_sprite.reset_animation();

    landed_planet=-1;

    quadtree_debris.clear_tree();
    quadtree_shots.clear_tree();
    quadtree_ships.clear_tree();
    quadtree_explosions.clear_tree();
    quadtree_items.clear_tree();
    quadtree_planets.clear_tree();

    frame=0;
    ship_spawn_check=0;
}

void Game::generate_world(){
    clear_world();

    contract_sprite.set_name("contract_indicator_arrow");
    no_contract_sprite.set_name("contract_indicator_none");

    rng.seed((uint32_t)time(0));

    Background::setup();

    world_width=10000.0;
    world_height=10000.0;

    //Generate star
    stars.push_back(Star(Coords<double>(world_width/2.0,world_height/2.0)));

    //Generate planets
    uint32_t planet_count=((world_width+world_height)/2.0)/2048.0;
    if(planet_count<2){
        planet_count=2;
    }
    for(uint32_t i=0;i<planet_count;i++){
        Sprite sprite;
        sprite.set_name("planet_"+Strings::num_to_string(rng.random_range(0,0)));

        uint32_t radius=uint32_t(sprite.get_width()/2.0);

        uint32_t x=rng.random_range(radius,(uint32_t)world_width-radius);
        uint32_t y=rng.random_range(radius,(uint32_t)world_height-radius);

        planets.push_back(Planet(sprite.name,Coords<double>((double)x,(double)y)));
    }

    //Generate debris
    uint32_t asteroid_count=((world_width+world_height)/2.0)/24.0;
    for(uint32_t i=0;i<asteroid_count;i++){
        string type="asteroid_"+Strings::num_to_string(rng.random_range(0,5));
        Sprite sprite;
        sprite.set_name(Game_Data::get_debris_type(type)->sprite);

        uint32_t x=rng.random_range(0,(uint32_t)world_width-(uint32_t)sprite.get_width());
        uint32_t y=rng.random_range(0,(uint32_t)world_height-(uint32_t)sprite.get_height());

        debris.push_back(Debris(type,Coords<double>((double)x,(double)y),rng.random_range(0,359),Vector(0.01*rng.random_range(0,75),rng.random_range(0,359))));
    }

    //Generate items
    uint32_t item_count=((world_width+world_height)/2.0)/256.0;
    for(uint32_t i=0;i<item_count;i++){
        string type=get_random_item_type();

        Sprite sprite;
        sprite.set_name(Game_Data::get_item_type(type)->sprite);

        uint32_t x=rng.random_range(0,(uint32_t)world_width-(uint32_t)sprite.get_width());
        uint32_t y=rng.random_range(0,(uint32_t)world_height-(uint32_t)sprite.get_height());

        items.push_back(Item(type,Coords<double>((double)x,(double)y),Vector(0.0,0.0),rng.random_range(0,359),Vector(0.0,0.0)));
    }

    ///QQQ Try to get number of planets desired, but don't allow any too near the star, and don't allow any to touch each other
    ///QQQ If debris is too near the star or touching other debris, skip it
    ///QQQ If item is too near the star or touching a planet, or touching another item, skip it
    ///QQQ If player is touching any debris, erase that debris
    ///QQQ If player is within vacuum range of any item, erase that item

    //Generate the player's ship
    string ship_type="player";
    Sprite ship_sprite;
    ship_sprite.set_name(Game_Data::get_ship_type(ship_type)->sprite);
    uint32_t random_planet_index=rng.random_range(0,planets.size()-1);
    const Planet& planet=planets[random_planet_index];
    ships.push_back(Ship(ship_type,Coords<double>(planet.get_circle().x-ship_sprite.get_width()/2.0,planet.get_circle().y-ship_sprite.get_height()/2.0),rng.random_range(0,359)));
    ships.back().ai_select_target(0,rng);
    ships.back().toggle_weapons();

    //Setup initial contract
    assign_new_contract(get_nearest_planet());

    quadtree_debris.setup(10,5,0,Collision_Rect<double>(0,0,world_width,world_height));
    quadtree_shots.setup(10,5,0,Collision_Rect<double>(0,0,world_width,world_height));
    quadtree_ships.setup(10,5,0,Collision_Rect<double>(0,0,world_width,world_height));
    quadtree_explosions.setup(10,5,0,Collision_Rect<double>(0,0,world_width,world_height));
    quadtree_items.setup(10,5,0,Collision_Rect<double>(0,0,world_width,world_height));
    quadtree_planets.setup(10,5,0,Collision_Rect<double>(0,0,world_width,world_height));

    generate_ships();
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

const Ship& Game::get_ship(uint32_t index){
    if(index<ships.size()){
        return ships[index];
    }
    else{
        Log::add_error("Error accessing ship '"+Strings::num_to_string(index)+"'");

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

const Explosion& Game::get_explosion(uint32_t index){
    if(index<explosions.size()){
        return explosions[index];
    }
    else{
        Log::add_error("Error accessing explosion '"+Strings::num_to_string(index)+"'");

        Engine::quit();
    }
}

const Item& Game::get_item(uint32_t index){
    if(index<items.size()){
        return items[index];
    }
    else{
        Log::add_error("Error accessing item '"+Strings::num_to_string(index)+"'");

        Engine::quit();
    }
}

const Star& Game::get_star(){
    if(!stars.empty()){
        return stars.front();
    }
    else{
        Log::add_error("Error accessing star");

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
    amount*=score_multiplier;

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

void Game::decrease_score_multiplier(uint64_t amount){
    if(amount<=score_multiplier){
        score_multiplier-=amount;
    }
    else{
        score_multiplier=0;
    }

    if(score_multiplier==0){
        score_multiplier++;
    }
}

uint32_t Game::get_nearest_planet(uint32_t ship_index){
    const Ship& ship=get_ship(ship_index);

    int32_t nearest_index=-1;
    double nearest_distance=0.0;

    for(size_t i=0;i<planets.size();i++){
        const Planet& planet=planets[i];

        double new_distance=Math::distance_between_points(ship.get_box().center_x(),ship.get_box().center_y(),planet.get_circle().x,planet.get_circle().y);

        if(nearest_index==-1 || new_distance<nearest_distance){
            nearest_index=i;
            nearest_distance=new_distance;
        }
    }

    return (uint32_t)nearest_index;
}

uint32_t Game::get_nearest_planet(){
    return get_nearest_planet(0);
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
    increase_score(Game_Constants::POINT_VALUE_CONTRACT);

    increase_score_multiplier(Game_Constants::SCORE_MULTIPLIER_INCREASE);

    uint32_t current_planet=contract;

    assign_new_contract(current_planet);

    landed_planet=-1;

    Game_Manager::paused=false;

    Engine::make_toast("Contract completed\nScore multiplier +"+Strings::num_to_string(Game_Constants::SCORE_MULTIPLIER_INCREASE));

    Sound_Manager::play_sound("contract_completed");
}

void Game::cancel_contract(){
    contract=-1;

    Engine::make_toast("Contract abandoned");
}

void Game::arrest_player(){
    clear_tractor();

    contract=-1;

    reset_notoriety();

    decrease_score_multiplier(Game_Constants::SCORE_MULTIPLIER_DECREASE);

    get_player().stop();

    Engine::make_toast("Arrested");

    Sound_Manager::play_sound("arrested");
}

void Game::build_upgrade_list(){
    upgrade_list.clear();

    //Begin with a list of all upgrades
    vector<string> upgrades=Game_Data::get_upgrade_names();

    //Erase the upgrades that the player currently has
    for(size_t i=0;i<upgrades.size();){
        if(get_player_const().has_upgrade(upgrades[i])){
            upgrades.erase(upgrades.begin()+i);
        }
        else{
            i++;
        }
    }

    for(uint32_t i=0;i<Game_Constants::UPGRADE_LIST_SIZE && upgrades.size()>0;i++){
        uint32_t upgrade=rng.random_range(0,upgrades.size()-1);

        upgrade_list.push_back(upgrades[upgrade]);

        upgrades.erase(upgrades.begin()+upgrade);
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

        Sound_Manager::play_sound("contract_acquired");
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

uint32_t Game::get_power(){
    return power;
}

bool Game::player_is_out_of_power(){
    return power==0;
}

void Game::increase_power(){
    power+=Game_Constants::ITEM_RESTORE_POWER*Engine::UPDATE_RATE;

    if(power>Game_Constants::MAX_POWER*Engine::UPDATE_RATE){
        power=Game_Constants::MAX_POWER*Engine::UPDATE_RATE;
    }
}

uint32_t Game::get_notoriety(){
    return notoriety;
}

bool Game::notoriety_tier_1(){
    return !notoriety_tier_2() && notoriety>=Game_Constants::NOTORIETY_TIER_1*Engine::UPDATE_RATE;
}

bool Game::notoriety_tier_2(){
    return notoriety>=Game_Constants::NOTORIETY_TIER_2*Engine::UPDATE_RATE;
}

void Game::increase_notoriety(){
    notoriety+=Game_Constants::NOTORIETY_INCREASE*Engine::UPDATE_RATE;

    if(notoriety>Game_Constants::NOTORIETY_MAX*Engine::UPDATE_RATE){
        notoriety=Game_Constants::NOTORIETY_MAX*Engine::UPDATE_RATE;
    }
}

void Game::reset_notoriety(){
    notoriety=0;
}

bool Game::is_player_tractored(){
    return player_tractored;
}

uint32_t Game::get_tractoring_ship_index(){
    return tractoring_ship;
}

double Game::get_tractor_angle(){
    return tractor_angle;
}

const Sprite& Game::get_tractor_sprite(){
    return tractor_sprite;
}

void Game::clear_tractor(){
    player_tractored=false;
    tractoring_ship=0;
    tractor_angle=0.0;
}

void Game::tractor_player(double angle,uint32_t ship_index){
    player_tractored=true;
    tractoring_ship=ship_index;
    tractor_angle=angle;
}

Vector Game::get_player_acceleration(){
    return player_acceleration;
}

void Game::set_player_acceleration(const Vector& acceleration){
    player_acceleration=acceleration;
}

void Game::create_effect(string sprite,double scale,const Coords<double>& position,string sound,const Vector& velocity,double angle,
                         const Vector& angular_velocity,uint32_t seconds,bool line,const Coords<double>& end_point){
    if(Game_Manager::effect_allowed()){
        effects.push_back(Effect(sprite,scale,position,sound,velocity,angle,angular_velocity,seconds,line,end_point));
    }
}

void Game::player_thrust(string direction){
    if(!get_player().is_disabled(true)){
        get_player().set_thrust_angle(direction);
    }
}

void Game::player_brake(bool brake){
    if(!get_player().is_disabled(true)){
        get_player().set_braking(brake);
    }
}

void Game::player_toggle_weapons(){
    get_player().toggle_weapons();

    Sound_Manager::play_sound("toggle_weapons");
}

void Game::player_add_upgrade(string name){
    get_player().add_upgrade(name);
}

void Game::player_remove_upgrade(string name){
    get_player().remove_upgrade(name);
}

void Game::disable_ship(uint32_t index){
    if(index<ships.size()){
        ships[index].disable();
    }
}

void Game::kill_shot(uint32_t index){
    if(index<shots.size()){
        shots[index].die();
    }
}

void Game::create_shot(uint32_t owner_index,string type,string faction,string firing_upgrade,const Coords<double>& position,double angle,int32_t damage_mod){
    shots.push_back(Shot(owner_index,type,faction,firing_upgrade,position,angle,damage_mod));
}

void Game::create_explosion(string sprite,string sound,const Coords<double>& position,int32_t damage,string faction){
    explosions.push_back(Explosion(sprite,sound,position,damage,faction));
}

string Game::get_random_item_type(){
    uint32_t random_item=rng.random_range(0,99);

    if(random_item<50){
        return "restore_hull";
    }
    else{
        return "restore_power";
    }
}

void Game::kill_item(uint32_t index){
    if(index<items.size()){
        items[index].die();
    }
}

void Game::create_item(const Coords<double>& position,const Vector& base_velocity){
    string type=get_random_item_type();

    Sprite sprite;
    sprite.set_name(Game_Data::get_item_type(type)->sprite);

    Vector velocity=base_velocity;
    velocity+=Vector(0.01*(double)rng.random_range((uint32_t)(Game_Constants::ITEM_START_VELOCITY_MIN*100.0),(uint32_t)(Game_Constants::ITEM_START_VELOCITY_MAX*100.0)),rng.random_range(0,359));

    items.push_back(Item(type,Coords<double>(position.x-sprite.get_width()/2.0,position.y-sprite.get_height()/2.0),velocity,
                         rng.random_range(0,359),Vector(0.01*rng.random_range(0,150),rng.random_range(0,359))));
}

void Game::generate_ships(){
    uint32_t ship_count=ships.size()-1;

    ///QQQ maximum number of ships determined by notoriety and score multiplier
    uint32_t MAX_SHIP_COUNT=50;

    for(uint32_t i=ship_count;i<MAX_SHIP_COUNT;i++){
        ///QQQ chance per ship to spawn is also determined by notoriety and score multiplier
        if(rng.random_range(0,99)<100){
            ///QQQ ship type is determined by player's current area and notoriety
            string ship_type="test_civilian";
            uint32_t random_ship=rng.random_range(0,99);
            if(random_ship>=25 && random_ship<50){
                ship_type="test_pirate";
            }
            else if(random_ship>=50 && random_ship<75){
                ship_type="test_bounty_hunter";
            }
            else if(random_ship>=75){
                ship_type="test_police";
            }

            Sprite ship_sprite;
            ship_sprite.set_name(Game_Data::get_ship_type(ship_type)->sprite);

            const Ship& player=get_player_const();

            double x=0.0;
            double y=0.0;

            ///QQQ weight side selection to prefer the contract direction, or if no contract, the player's current facing direction
            //Select a random side of the player, and select a random point within that side that conforms to the minimum and maximum ship spawn distances
            string random_side=Game_Manager::get_random_direction_cardinal(rng);
            if(random_side=="left"){
                x=player.get_box().x-(double)rng.random_range(Game_Constants::SHIP_SPAWN_DISTANCE_MIN,Game_Constants::SHIP_SPAWN_DISTANCE_MAX);
                if(rng.random_range(0,99)<50){
                    y=player.get_box().y-(double)rng.random_range(0,Game_Constants::SHIP_SPAWN_DISTANCE_MAX);
                }
                else{
                    y=player.get_box().y+(double)rng.random_range(0,Game_Constants::SHIP_SPAWN_DISTANCE_MAX);
                }
            }
            else if(random_side=="right"){
                x=player.get_box().x+player.get_box().w+(double)rng.random_range(Game_Constants::SHIP_SPAWN_DISTANCE_MIN,Game_Constants::SHIP_SPAWN_DISTANCE_MAX);
                if(rng.random_range(0,99)<50){
                    y=player.get_box().y-(double)rng.random_range(0,Game_Constants::SHIP_SPAWN_DISTANCE_MAX);
                }
                else{
                    y=player.get_box().y+(double)rng.random_range(0,Game_Constants::SHIP_SPAWN_DISTANCE_MAX);
                }
            }
            else if(random_side=="up"){
                y=player.get_box().y-(double)rng.random_range(Game_Constants::SHIP_SPAWN_DISTANCE_MIN,Game_Constants::SHIP_SPAWN_DISTANCE_MAX);
                if(rng.random_range(0,99)<50){
                    x=player.get_box().x-(double)rng.random_range(0,Game_Constants::SHIP_SPAWN_DISTANCE_MAX);
                }
                else{
                    x=player.get_box().x+(double)rng.random_range(0,Game_Constants::SHIP_SPAWN_DISTANCE_MAX);
                }
            }
            else if(random_side=="down"){
                y=player.get_box().y+player.get_box().h+(double)rng.random_range(Game_Constants::SHIP_SPAWN_DISTANCE_MIN,Game_Constants::SHIP_SPAWN_DISTANCE_MAX);
                if(rng.random_range(0,99)<50){
                    x=player.get_box().x-(double)rng.random_range(0,Game_Constants::SHIP_SPAWN_DISTANCE_MAX);
                }
                else{
                    x=player.get_box().x+(double)rng.random_range(0,Game_Constants::SHIP_SPAWN_DISTANCE_MAX);
                }
            }

            //Ensure the point is inside the world
            if(x<0.0){
                x+=(double)Game_Constants::SHIP_SPAWN_DISTANCE_MAX*2.0;
            }
            if(y<0.0){
                y+=(double)Game_Constants::SHIP_SPAWN_DISTANCE_MAX*2.0;
            }
            if(x>=world_width){
                x-=(double)Game_Constants::SHIP_SPAWN_DISTANCE_MAX*2.0;
            }
            if(y>=world_height){
                y-=(double)Game_Constants::SHIP_SPAWN_DISTANCE_MAX*2.0;
            }

            ///QQQ make sure the ship won't spawn inside debris or the star

            ships.push_back(Ship(ship_type,Coords<double>(x,y),rng.random_range(0,359)));
            ships.back().ai_select_target((uint32_t)ships.size()-1,rng);
        }
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

    if(++ship_spawn_check>=Game_Constants::SHIP_SPAWN_RATE*Engine::UPDATE_RATE){
        ship_spawn_check=0;

        generate_ships();
    }

    if(power>0 && --power==0){
        game_over();
    }

    if(notoriety>0){
        notoriety--;
    }

    clear_tractor();

    quadtree_debris.clear_tree();
    for(size_t i=0;i<debris.size();i++){
        quadtree_debris.insert_object(debris[i].get_collision_box(),(uint32_t)i);
    }

    quadtree_shots.clear_tree();
    for(size_t i=0;i<shots.size();i++){
        if(shots[i].is_alive()){
            quadtree_shots.insert_object(shots[i].get_collision_box(),(uint32_t)i);
        }
    }

    quadtree_ships.clear_tree();
    for(size_t i=0;i<ships.size();i++){
        if(ships[i].is_alive()){
            quadtree_ships.insert_object(ships[i].get_collision_box(),(uint32_t)i);
        }
    }

    quadtree_explosions.clear_tree();
    for(size_t i=0;i<explosions.size();i++){
        if(explosions[i].is_alive()){
            quadtree_explosions.insert_object(explosions[i].get_circle(),(uint32_t)i);
        }
    }

    quadtree_items.clear_tree();
    for(size_t i=0;i<items.size();i++){
        if(items[i].is_alive()){
            quadtree_items.insert_object(items[i].get_box(),(uint32_t)i);
        }
    }

    quadtree_planets.clear_tree();
    for(size_t i=0;i<planets.size();i++){
        quadtree_planets.insert_object(planets[i].get_circle(),(uint32_t)i);
    }
}

void Game::ai(){
    for(size_t i=1;i<ships.size();i++){
        ships[i].ai(quadtree_ships,quadtree_planets,frame,(uint32_t)i,rng);
    }
}

void Game::movement(){
    for(size_t i=0;i<debris.size();i++){
        debris[i].rotation();
    }

    for(size_t i=0;i<items.size();i++){
        items[i].accelerate();
    }

    for(size_t i=0;i<items.size();i++){
        items[i].rotation();
        items[i].movement();
    }

    for(size_t i=0;i<ships.size();i++){
        ships[i].land(i==0);

        ships[i].regenerate_shields(i==0);
        ships[i].cooldown(quadtree_ships,quadtree_shots,rng,(uint32_t)i);
        ships[i].calculate_laser_target(quadtree_ships,(uint32_t)i);

        ships[i].accelerate(i==0,frame);
    }

    for(size_t i=0;i<ships.size();i++){
        ships[i].movement((uint32_t)i,quadtree_debris,quadtree_shots,quadtree_explosions,quadtree_items,rng);
    }

    for(size_t i=0;i<shots.size();i++){
        shots[i].cooldown();

        shots[i].accelerate(quadtree_ships);
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
        if(!ships[i].is_alive() || ships[i].get_distance_to_player()>=Game_Constants::DESPAWN_DISTANCE){
            uint32_t dying_ship_index=(uint32_t)i;

            for(size_t shots_index=0;shots_index<shots.size();shots_index++){
                if(shots[shots_index].is_alive()){
                    shots[shots_index].notify_of_ship_death(dying_ship_index);
                }
            }

            for(size_t ships_index=1;ships_index<ships.size();ships_index++){
                if(ships[ships_index].is_alive()){
                    ships[ships_index].notify_of_ship_death(dying_ship_index);
                }
            }

            if(is_player_tractored()){
                if(dying_ship_index==tractoring_ship){
                    clear_tractor();
                }
                else if(dying_ship_index<tractoring_ship){
                    tractoring_ship--;
                }
            }

            ships.erase(ships.begin()+i);
        }
        else{
            i++;
        }
    }

    for(size_t i=0;i<shots.size();){
        if(!shots[i].is_alive() || shots[i].get_distance_to_player()>=Game_Constants::DESPAWN_DISTANCE){
            shots.erase(shots.begin()+i);
        }
        else{
            i++;
        }
    }

    for(size_t i=0;i<explosions.size();){
        if(!explosions[i].is_alive() || explosions[i].get_distance_to_player()>=Game_Constants::DESPAWN_DISTANCE){
            for(size_t ships_index=0;ships_index<ships.size();ships_index++){
                if(ships[ships_index].is_alive()){
                    ships[ships_index].notify_of_explosion_death((uint32_t)i);
                }
            }

            explosions.erase(explosions.begin()+i);
        }
        else{
            i++;
        }
    }

    for(size_t i=0;i<items.size();){
        if(!items[i].is_alive()){
            items.erase(items.begin()+i);
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

    for(size_t i=0;i<items.size();i++){
        items[i].animate();
    }

    for(size_t i=0;i<ships.size();i++){
        ships[i].animate();
    }

    for(size_t i=0;i<shots.size();i++){
        shots[i].animate();
    }

    for(size_t i=0;i<explosions.size();i++){
        explosions[i].animate();
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

    if(is_player_tractored()){
        tractor_sprite.animate();
    }
}

void Game::render(){
    for(size_t i=0;i<stars.size();i++){
        stars[i].render();
    }

    for(size_t i=0;i<planets.size();i++){
        planets[i].render();
    }

    for(size_t i=0;i<debris.size();i++){
        debris[i].render();
    }

    for(size_t i=0;i<items.size();i++){
        items[i].render();
    }

    for(size_t i=0;i<ships.size();i++){
        ships[i].render(is_player_tractored() && (uint32_t)i==tractoring_ship);
        ships[i].render_laser(i==0);
    }

    for(size_t i=0;i<shots.size();i++){
        shots[i].render();
    }

    for(size_t i=0;i<explosions.size();i++){
        explosions[i].render();
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
                               box_contract_indicator.get_angle_to_circ(planet.get_circle()));
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
    Render::render_rectangle(0,0,Game_Window::width(),Game_Window::height(),1.0,"space");

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
