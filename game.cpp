/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game.h"
#include "game_data.h"
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
#include <unordered_set>

using namespace std;

#ifdef GAME_OS_ANDROID
const uint64_t Game::UINT64_MAX=18446744073709551615;
const uint32_t Game::UINT32_MAX=4294967295U;
#endif

Title Game::title;

Background Game::background_stars;
Background Game::background_planetary;

double Game::background_opacity_planetary=0.0;

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

Sprite Game::police_lights_sprite;
Vector Game::police_lights_angular_velocity;
double Game::police_lights_angle=0.0;

int32_t Game::landed_planet=0;

uint64_t Game::score=0;
uint64_t Game::score_multiplier=0;

uint32_t Game::power=0;

uint32_t Game::notoriety=0;

uint32_t Game::kills=0;

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
uint32_t Game::item_spawn_check=0;

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
    background_stars.unload();
    background_planetary.unload();

    background_opacity_planetary=0.0;

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

    kills=0;

    clear_tractor();
    tractor_sprite.set_name("tractor_beam");

    player_acceleration*=0.0;

    upgrade_list.clear();

    contract=-1;

    contract_sprite.set_name("contract_indicator_arrow");
    no_contract_sprite.set_name("contract_indicator_none");

    police_lights_sprite.set_name("police_lights");
    police_lights_angular_velocity.magnitude=Game_Constants::POLICE_LIGHTS_ANGULAR_SPEED;
    police_lights_angular_velocity.direction=0.0;
    police_lights_angle=0.0;

    landed_planet=-1;

    quadtree_debris.clear_tree();
    quadtree_shots.clear_tree();
    quadtree_ships.clear_tree();
    quadtree_explosions.clear_tree();
    quadtree_items.clear_tree();
    quadtree_planets.clear_tree();

    frame=0;
    ship_spawn_check=0;
    item_spawn_check=0;
}

void Game::generate_world(){
    clear_world();

    rng.seed((uint32_t)time(0));

    background_stars.setup("stars",rng);
    background_planetary.setup("planetary",rng);

    background_opacity_planetary=1.0;

    world_width=10000.0;
    world_height=10000.0;

    uint32_t max_attempts=10000;

    //Generate star
    stars.push_back(Star(Coords<double>(world_width/2.0,world_height/2.0)));

    //Generate planets
    uint32_t minimum_planets=2;
    uint32_t planet_count=((world_width+world_height)/2.0)/1024.0;
    if(planet_count<minimum_planets){
        planet_count=minimum_planets;
    }

    for(uint32_t i=0,attempts=0;i<planet_count && (planets.size()<minimum_planets || attempts<max_attempts);attempts++){
        Sprite sprite;
        sprite.set_name("planet_"+Strings::num_to_string(rng.random_range(0,0)));

        uint32_t radius=uint32_t(sprite.get_width()/2.0);

        uint32_t x=rng.random_range(radius,(uint32_t)world_width-radius);
        uint32_t y=rng.random_range(radius,(uint32_t)world_height-radius);

        Coords<double> position((double)x,(double)y);

        double minimum_distance_from_star=Game_Constants::MINIMUM_GEN_DISTANCE_FROM_STAR+Game_Constants::STAR_RADIUS+(double)radius;

        if(attempts>=max_attempts || Math::get_distance_between_points(position,stars.back().get_circle().get_center())>=minimum_distance_from_star){
            bool too_close_to_planet=false;

            for(size_t j=0;j<planets.size();j++){
                double minimum_distance_between_planets=Game_Constants::MINIMUM_GEN_DISTANCE_BETWEEN_PLANETS+planets[j].get_circle().r+(double)radius;

                if(Math::get_distance_between_points(position,planets[j].get_circle().get_center())<minimum_distance_between_planets){
                    too_close_to_planet=true;

                    break;
                }
            }

            if(attempts>=max_attempts || !too_close_to_planet){
                planets.push_back(Planet(sprite.name,position));

                i++;
            }
        }
    }

    //Generate debris
    uint32_t asteroid_count=((world_width+world_height)/2.0)/8.0;

    for(uint32_t i=0;i<asteroid_count;i++){
        string type="asteroid_"+Strings::num_to_string(rng.random_range(0,2));
        Sprite sprite;
        sprite.set_name(Game_Data::get_debris_type(type)->sprite);

        uint32_t x=rng.random_range(0,(uint32_t)world_width-(uint32_t)sprite.get_width());
        uint32_t y=rng.random_range(0,(uint32_t)world_height-(uint32_t)sprite.get_height());

        Coords<double> position((double)x,(double)y);

        double radius=(sprite.get_width()+sprite.get_height())/4.0;
        double minimum_distance_from_star=Game_Constants::MINIMUM_GEN_DISTANCE_FROM_STAR+Game_Constants::STAR_RADIUS+radius;

        if(Math::get_distance_between_points(position,stars.back().get_circle().get_center())>=minimum_distance_from_star){
            bool touching_debris=false;

            for(size_t j=0;j<debris.size();j++){
                if(Collision::check_rect(Collision_Rect<double>((double)x,(double)y,sprite.get_width(),sprite.get_height()),debris[j].get_box())){
                    touching_debris=true;

                    break;
                }
            }

            if(!touching_debris){
                debris.push_back(Debris(type,position,rng.random_range(0,359),Vector(0.01*rng.random_range(0,75),rng.random_range(0,359))));
            }
        }
    }

    //Generate the player's ship
    string ship_type="player_0";
    Sprite ship_sprite;
    ship_sprite.set_name(Game_Data::get_ship_type(ship_type)->sprite);
    uint32_t random_planet_index=rng.random_range(0,planets.size()-1);
    const Planet& planet=planets[random_planet_index];
    ships.push_back(Ship(ship_type,Coords<double>(planet.get_circle().x-ship_sprite.get_width()/2.0,planet.get_circle().y-ship_sprite.get_height()/2.0),rng.random_range(0,359)));
    ships.back().ai_select_target(0,rng);
    ships.back().toggle_weapons();

    //Clear player's starting area of debris
    for(size_t i=0;i<debris.size();){
        Coords<double> player_position=ships.back().get_box().get_center();

        if(Collision::check_rect(Collision_Rect<double>(player_position.x-Game_Constants::GEN_PLAYER_START_CLEAN_DISTANCE,
                                                        player_position.y-Game_Constants::GEN_PLAYER_START_CLEAN_DISTANCE,
                                                        Game_Constants::GEN_PLAYER_START_CLEAN_DISTANCE*2.0,
                                                        Game_Constants::GEN_PLAYER_START_CLEAN_DISTANCE*2.0),debris[i].get_box())){
            debris.erase(debris.begin()+i);
        }
        else{
            i++;
        }
    }

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

uint32_t Game::get_item_count(){
    return items.size();
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

uint32_t Game::get_explosion_count(){
    return explosions.size();
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

Title& Game::get_title(){
    return title;
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

        double new_distance=Math::get_distance_between_points(ship.get_box().get_center(),planet.get_circle().get_center());

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

bool Game::is_player_in_deep_space(){
    const Ship& player=get_player_const();

    int32_t nearest_index=-1;
    double nearest_distance=0.0;

    for(size_t i=0;i<planets.size();i++){
        const Planet& planet=planets[i];

        double new_distance=Math::get_distance_between_points(player.get_box().get_center(),planet.get_circle().get_center());

        if(nearest_index==-1 || new_distance<nearest_distance){
            nearest_index=i;
            nearest_distance=new_distance;
        }
    }

    const Planet& planet=get_planet((uint32_t)nearest_index);

    return nearest_distance>(player.get_box().w+player.get_box().h)/4.0+planet.get_circle().r+Game_Constants::PLANETARY_SPACE_RANGE;
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

    generate_ships();
    generate_items();

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

    //Erase the upgrades that are not allowed
    for(size_t i=0;i<upgrades.size();){
        if(Game_Data::get_upgrade_type(upgrades[i])->banned || get_player_const().has_upgrade(upgrades[i])){
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

const Sprite& Game::get_police_lights_sprite(){
    return police_lights_sprite;
}

double Game::get_police_lights_angle(){
    return police_lights_angle;
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

void Game::use_power(uint32_t amount){
    if(amount<=power){
        power-=amount;
    }
    else{
        power=0;
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

void Game::increase_notoriety(uint32_t amount){
    notoriety+=amount*Engine::UPDATE_RATE;

    if(notoriety>Game_Constants::NOTORIETY_MAX*Engine::UPDATE_RATE){
        notoriety=Game_Constants::NOTORIETY_MAX*Engine::UPDATE_RATE;
    }
}

void Game::reset_notoriety(){
    notoriety=0;
}

uint32_t Game::get_kills(){
    return kills;
}

void Game::add_kill(){
    kills++;
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

void Game::create_effect(string sprite,bool fade,double scale,const Coords<double>& position,string sound,const Vector& velocity,double angle,
                         const Vector& angular_velocity,uint32_t seconds,bool line,const Coords<double>& end_point,string color){
    if(Game_Manager::effect_allowed()){
        effects.push_back(Effect(sprite,fade,scale,position,sound,velocity,angle,angular_velocity,seconds,line,end_point,color));
    }
}

void Game::player_thrust(string direction){
    if(!get_player().is_disabled(true)){
        if(!get_player().is_warping()){
            get_player().set_thrust_angle(direction);
        }
        else{
            get_player().set_thrusting(true);
        }
    }
    else{
        get_player().set_thrust_angle("none");
    }
}

void Game::player_brake(bool brake){
    if(!get_player().is_disabled(true) && !get_player().is_warping()){
        get_player().set_braking(brake);
    }
    else{
        get_player().set_braking(false);
    }
}

void Game::player_toggle_weapons(){
    get_player().toggle_weapons();

    Sound_Manager::play_sound("toggle_weapons");
}

void Game::player_use_active(){
    get_player().use_active(true);
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

void Game::create_explosion(string sprite,string sound,const Coords<double>& position,int32_t damage,string faction,bool scan,bool emp){
    explosions.push_back(Explosion(sprite,sound,position,damage,faction,scan,emp));
}

string Game::get_random_item_type(){
    double hull_percentage=get_player_const().get_hull()/get_player_const().get_hull_max();
    double power_percentage=power/(Game_Constants::MAX_POWER*Engine::UPDATE_RATE);

    uint32_t weighted_towards=0;
    if(power_percentage<hull_percentage){
        weighted_towards=1;
    }

    uint32_t random_item=rng.weighted_random_range(0,1,weighted_towards);

    if(random_item==0){
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

bool Game::collides_with_game_world(const Collision_Rect<double>& box){
    for(size_t i=0;i<stars.size();i++){
        if(Collision::check_rect_circ(box,stars[i].get_circle())){
            return true;
        }
    }

    vector<uint32_t> nearby_debris;
    quadtree_debris.get_objects(nearby_debris,box);

    unordered_set<uint32_t> collisions;

    for(size_t i=0;i<nearby_debris.size();i++){
        if(!collisions.count(nearby_debris[i])){
            collisions.emplace(nearby_debris[i]);

            if(Collision::check_rect(box,get_debris(nearby_debris[i]).get_box())){
                return true;
            }
        }
    }

    return false;
}

Coords<double> Game::get_spawn_point(double width,double height){
    uint32_t attempts=0;

    double x=-1.0;
    double y=-1.0;

    do{
        const Ship& player=get_player_const();

        double angle_of_interest=0.0;

        if(player_has_contract()){
            const Planet& planet=get_contract_planet();

            //Angle of interest is the angle from the player to the contract planet
            angle_of_interest=player.get_box().get_angle_to_circ(planet.get_circle());
        }
        else{
            //Angle of interest is simply the player's current facing angle
            angle_of_interest=player.get_angle();
        }

        uint32_t preferred_side=0;

        Math::clamp_angle(angle_of_interest);

        if(angle_of_interest>=315.0 && angle_of_interest<=45.0){
            preferred_side=2;
        }
        else if(angle_of_interest>=135.0 && angle_of_interest<=225.0){
            preferred_side=0;
        }
        else if(angle_of_interest>45.0 && angle_of_interest<135.0){
            preferred_side=1;
        }
        else{
            preferred_side=3;
        }

        uint32_t random_side_num=rng.weighted_random_range(0,3,preferred_side);

        string random_side="";

        if(random_side_num==0){
            random_side="left";
        }
        else if(random_side_num==1){
            random_side="up";
        }
        else if(random_side_num==2){
            random_side="right";
        }
        else{
            random_side="down";
        }

        if(random_side=="left"){
            x=player.get_box().x-(double)rng.random_range(Game_Constants::SPAWN_DISTANCE_MIN,Game_Constants::SPAWN_DISTANCE_MAX);
            if(rng.random_range(0,99)<50){
                y=player.get_box().y-(double)rng.random_range(0,Game_Constants::SPAWN_DISTANCE_MAX);
            }
            else{
                y=player.get_box().y+(double)rng.random_range(0,Game_Constants::SPAWN_DISTANCE_MAX);
            }
        }
        else if(random_side=="right"){
            x=player.get_box().x+player.get_box().w+(double)rng.random_range(Game_Constants::SPAWN_DISTANCE_MIN,Game_Constants::SPAWN_DISTANCE_MAX);
            if(rng.random_range(0,99)<50){
                y=player.get_box().y-(double)rng.random_range(0,Game_Constants::SPAWN_DISTANCE_MAX);
            }
            else{
                y=player.get_box().y+(double)rng.random_range(0,Game_Constants::SPAWN_DISTANCE_MAX);
            }
        }
        else if(random_side=="up"){
            y=player.get_box().y-(double)rng.random_range(Game_Constants::SPAWN_DISTANCE_MIN,Game_Constants::SPAWN_DISTANCE_MAX);
            if(rng.random_range(0,99)<50){
                x=player.get_box().x-(double)rng.random_range(0,Game_Constants::SPAWN_DISTANCE_MAX);
            }
            else{
                x=player.get_box().x+(double)rng.random_range(0,Game_Constants::SPAWN_DISTANCE_MAX);
            }
        }
        else if(random_side=="down"){
            y=player.get_box().y+player.get_box().h+(double)rng.random_range(Game_Constants::SPAWN_DISTANCE_MIN,Game_Constants::SPAWN_DISTANCE_MAX);
            if(rng.random_range(0,99)<50){
                x=player.get_box().x-(double)rng.random_range(0,Game_Constants::SPAWN_DISTANCE_MAX);
            }
            else{
                x=player.get_box().x+(double)rng.random_range(0,Game_Constants::SPAWN_DISTANCE_MAX);
            }
        }

        //Ensure the point is inside the world
        if(x<0.0){
            x+=(double)Game_Constants::SPAWN_DISTANCE_MAX*2.0;
        }
        if(y<0.0){
            y+=(double)Game_Constants::SPAWN_DISTANCE_MAX*2.0;
        }
        if(x>=world_width){
            x-=(double)Game_Constants::SPAWN_DISTANCE_MAX*2.0;
        }
        if(y>=world_height){
            y-=(double)Game_Constants::SPAWN_DISTANCE_MAX*2.0;
        }
    }while(++attempts<Game_Constants::MAX_ATTEMPTS_SPAWN && collides_with_game_world(Collision_Rect<double>(x,y,width,height)));

    //If a valid point was not found
    if(attempts==Game_Constants::MAX_ATTEMPTS_SPAWN){
        x=-1.0;
        y=-1.0;
    }

    return Coords<double>(x,y);
}

void Game::generate_ships(){
    uint64_t ship_count=ships.size()-1;

    uint64_t desired_ships=score_multiplier*Game_Constants::DESIRED_SHIPS_MULTIPLIER;
    if(desired_ships>Game_Constants::DESIRED_SHIPS_MAX){
        desired_ships=Game_Constants::DESIRED_SHIPS_MAX;
    }

    if(ship_count<=desired_ships){
        desired_ships-=ship_count;
    }
    else{
        desired_ships=0;
    }

    bool player_in_deep_space=is_player_in_deep_space();

    for(uint64_t i=0;i<desired_ships;i++){
        uint32_t tier=0;

        if(score_multiplier>=Game_Constants::MIN_SCORE_MULTIPLIER_TIER_1){
            uint32_t score_multiplier_32=score_multiplier<(uint64_t)UINT32_MAX ? (uint32_t)score_multiplier : UINT32_MAX;

            uint32_t weight=score_multiplier_32/Game_Constants::SCORE_MULTIPLIER_TIER_DENOMINATOR;
            if(weight<2){
                weight=2;
            }
            else if(weight>256){
                weight=256;
            }

            if(score_multiplier>=Game_Constants::MIN_SCORE_MULTIPLIER_TIER_2){
                tier=rng.weighted_random_range(0,2,2,weight);
            }
            else{
                tier=rng.weighted_random_range(0,1,1,weight);
            }
        }

        vector<string> types;

        for(uint32_t j=0;j<Game_Constants::SHIP_WEIGHT_CIVILIAN;j++){
            types.push_back("civilian_0");
        }

        uint32_t ship_weight_bounty_hunter=0;
        if(notoriety_tier_2()){
            ship_weight_bounty_hunter=Game_Constants::SHIP_WEIGHT_BOUNTY_HUNTER_NOTORIETY_TIER_2;
        }
        else if(notoriety_tier_1()){
            ship_weight_bounty_hunter=Game_Constants::SHIP_WEIGHT_BOUNTY_HUNTER_NOTORIETY_TIER_1;
        }

        for(uint32_t j=0;j<ship_weight_bounty_hunter;j++){
            types.push_back("bounty_hunter_"+Strings::num_to_string(tier));
        }

        if(player_in_deep_space){
            for(uint32_t j=0;j<Game_Constants::SHIP_WEIGHT_PIRATE;j++){
                types.push_back("pirate_"+Strings::num_to_string(tier));
            }
        }
        else{
            uint32_t ship_weight_police=Game_Constants::SHIP_WEIGHT_POLICE_NOTORIETY_TIER_0;
            if(notoriety_tier_2()){
                ship_weight_police=Game_Constants::SHIP_WEIGHT_POLICE_NOTORIETY_TIER_2;
            }
            else if(notoriety_tier_1()){
                ship_weight_police=Game_Constants::SHIP_WEIGHT_POLICE_NOTORIETY_TIER_1;
            }

            for(uint32_t j=0;j<ship_weight_police;j++){
                types.push_back("police_"+Strings::num_to_string(tier));
            }
        }

        string type=types[rng.random_range(0,types.size()-1)];

        Sprite sprite;
        sprite.set_name(Game_Data::get_ship_type(type)->sprite);

        Coords<double> spawn_point=get_spawn_point(sprite.get_width(),sprite.get_height());

        if(spawn_point.x>=0.0 && spawn_point.y>=0.0){
            ships.push_back(Ship(type,spawn_point,rng.random_range(0,359)));
            ships.back().ai_select_target((uint32_t)ships.size()-1,rng);
        }
    }
}

void Game::generate_items(){
    uint64_t item_count=items.size();

    uint64_t desired_items=uint64_t(Game_Constants::DESIRED_ITEMS_BASE/((double)score_multiplier*Game_Constants::DESIRED_ITEMS_SCORE_MULTIPLIER_ADJUSTMENT));

    if(item_count<=desired_items){
        desired_items-=item_count;
    }
    else{
        desired_items=0;
    }

    for(uint64_t i=0;i<desired_items;i++){
        string type=get_random_item_type();

        Sprite sprite;
        sprite.set_name(Game_Data::get_item_type(type)->sprite);

        Coords<double> spawn_point=get_spawn_point(sprite.get_width(),sprite.get_height());

        if(spawn_point.x>=0.0 && spawn_point.y>=0.0){
            items.push_back(Item(type,spawn_point,Vector(0.0,0.0),rng.random_range(0,359),Vector(0.0,0.0)));
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

    if(++item_spawn_check>=Game_Constants::ITEM_SPAWN_RATE*Engine::UPDATE_RATE){
        item_spawn_check=0;

        generate_items();
    }

    if(power>0 && --power==0){
        game_over();
    }

    if(notoriety>0){
        notoriety--;
    }

    clear_tractor();

    for(size_t i=0;i<ships.size();i++){
        ships[i].check_processing_range(i==0);
    }

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

        ships[i].take_star_damage(i==0,rng);
        ships[i].regenerate_shields(i==0);
        ships[i].drain_power(i==0);
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
        if(!items[i].is_alive() || items[i].get_distance_to_player()>=Game_Constants::DESPAWN_DISTANCE){
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

    police_lights_sprite.animate();
    if(police_lights_angular_velocity.direction>=0 && police_lights_angular_velocity.direction<180){
        police_lights_angle+=police_lights_angular_velocity.magnitude;
    }
    else{
        police_lights_angle-=police_lights_angular_velocity.magnitude;
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
        ships[i].render((is_player_tractored() && (uint32_t)i==tractoring_ship),i==0);
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
    background_stars.update(Game_Manager::camera_delta_x,Game_Manager::camera_delta_y);
    background_planetary.update(Game_Manager::camera_delta_x,Game_Manager::camera_delta_y);

    if(is_player_in_deep_space()){
        if(background_opacity_planetary>0.0){
            background_opacity_planetary-=Game_Constants::BACKGROUND_FADE_RATE;

            if(background_opacity_planetary<0.0){
                background_opacity_planetary=0.0;
            }
        }
    }
    else{
        if(background_opacity_planetary<1.0){
            background_opacity_planetary+=Game_Constants::BACKGROUND_FADE_RATE;

            if(background_opacity_planetary>1.0){
                background_opacity_planetary=1.0;
            }
        }
    }
}

void Game::render_background(){
    Render::render_rectangle(0.0,0.0,Game_Window::width(),Game_Window::height(),1.0,"space");

    background_stars.render(1.0);

    if(background_opacity_planetary>0.0){
        background_planetary.render(background_opacity_planetary);
    }
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
