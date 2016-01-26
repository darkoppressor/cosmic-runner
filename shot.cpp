/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "shot.h"
#include "game_data.h"
#include "game.h"
#include "game_constants.h"

#include <engine.h>
#include <game_manager.h>
///QQQ includes
///#include <render.h>
///

#include <unordered_set>

using namespace std;

Shot::Shot(uint32_t new_owner_index,string new_type,string new_faction,string new_firing_upgrade,const Coords<double>& position,double new_angle){
    owner_index=(int32_t)new_owner_index;

    type=new_type;

    faction=new_faction;

    box.x=position.x;
    box.y=position.y;

    angle=new_angle;

    sprite.set_name(get_shot_type()->sprite);

    box.w=sprite.get_width();
    box.h=sprite.get_height();

    alive=true;

    firing_upgrade=new_firing_upgrade;
}

Shot_Type* Shot::get_shot_type() const{
    return Game_Data::get_shot_type(type);
}

Collision_Rect<double> Shot::get_box() const{
    return box;
}

Vector Shot::get_velocity() const{
    return velocity;
}

Collision_Rect<double> Shot::get_collision_box() const{
    return Collision_Rect<double>(box.x+box.w/2.0-box.w*get_shot_type()->collision_percentage/2.0,box.y+box.h/2.0-box.h*get_shot_type()->collision_percentage/2.0,
                                  box.w*get_shot_type()->collision_percentage,box.h*get_shot_type()->collision_percentage);
}

bool Shot::is_alive() const{
    return alive;
}

double Shot::get_distance_to_player() const{
    const Ship& player=Game::get_player_const();

    return Math::distance_between_points(box.center_x(),box.center_y(),player.get_box().center_x(),player.get_box().center_y());
}

bool Shot::has_owner() const{
    return owner_index>=0;
}

uint32_t Shot::get_owner_index() const{
    return (uint32_t)owner_index;
}

void Shot::clear_owner(){
    owner_index=-1;
}

void Shot::notify_of_ship_death(uint32_t index){
    if(has_owner()){
        if(get_owner_index()==index){
            clear_owner();
        }
        else if(index<get_owner_index()){
            owner_index--;
        }
    }
}

Upgrade* Shot::get_firing_upgrade() const{
    return Game_Data::get_upgrade_type(firing_upgrade);
}

void Shot::die(){
    if(is_alive()){
        alive=false;

        if(get_shot_type()->death_sprite.length()>0){
            Game::create_effect(get_shot_type()->death_sprite,1.0,Coords<double>(box.center_x(),box.center_y()),get_shot_type()->death_sound,Vector(),0.0,Vector(),0);
        }
    }
}

void Shot::thrust(const Quadtree<double,uint32_t>& quadtree_ships){
    if(get_shot_type()->homing){
        Collision_Rect<double> box_targeting=box;

        box_targeting.x-=Game_Constants::MISSILE_HOMING_RANGE;
        box_targeting.y-=Game_Constants::MISSILE_HOMING_RANGE;
        box_targeting.w+=Game_Constants::MISSILE_HOMING_RANGE*2.0;
        box_targeting.h+=Game_Constants::MISSILE_HOMING_RANGE*2.0;

        vector<uint32_t> nearby_ships;
        quadtree_ships.get_objects(nearby_ships,box_targeting);

        vector<uint32_t> valid_targets;

        unordered_set<uint32_t> collisions;

        //Find all valid targets
        for(size_t i=0;i<nearby_ships.size();i++){
            if(!collisions.count(nearby_ships[i])){
                collisions.emplace(nearby_ships[i]);

                const Ship& ship=Game::get_ship(nearby_ships[i]);

                if(ship.is_alive() && !ship.is_landing() && (nearby_ships[i]!=0 || !Game::player_is_landed())){
                    Collision_Rect<double> box_ship=ship.get_box();

                    if(faction!=ship.get_faction() && Collision::check_rect(box_targeting,box_ship)){
                        valid_targets.push_back(nearby_ships[i]);
                    }
                }
            }
        }

        //Find nearest valid target
        int32_t nearest_index=-1;
        double nearest_distance=0.0;

        for(size_t i=0;i<valid_targets.size();i++){
            const Ship& ship=Game::get_ship(valid_targets[i]);

            double new_distance=Math::distance_between_points(box.center_x(),box.center_y(),ship.get_box().center_x(),ship.get_box().center_y());

            if(nearest_index==-1 || new_distance<nearest_distance){
                nearest_index=valid_targets[i];
                nearest_distance=new_distance;
            }
        }

        //If a nearest valid target was found
        if(nearest_index>=0){
            const Ship& ship=Game::get_ship((uint32_t)nearest_index);

            angle=box.get_angle_to_rect(ship.get_box());
        }
    }

    Vector thrust_force(get_firing_upgrade()->thrust_accel,angle);

    force+=thrust_force;
}

void Shot::accelerate(const Quadtree<double,uint32_t>& quadtree_ships){
    if(is_alive()){
        thrust(quadtree_ships);

        Vector acceleration=force/get_shot_type()->mass;

        velocity+=acceleration;

        if(velocity.magnitude>get_firing_upgrade()->max_speed){
            velocity.magnitude=get_firing_upgrade()->max_speed;
        }
        else if(velocity.magnitude<-get_firing_upgrade()->max_speed){
            velocity.magnitude=-get_firing_upgrade()->max_speed;
        }

        force*=0.0;
    }
}

void Shot::movement(const Quadtree<double,uint32_t>& quadtree_debris){
    if(is_alive()){
        Vector_Components vc=velocity.get_components();

        box.x+=vc.a/(double)Engine::UPDATE_RATE;
        box.y+=vc.b/(double)Engine::UPDATE_RATE;

        Collision_Rect<double> box_collision=get_collision_box();

        vector<uint32_t> nearby_debris;
        quadtree_debris.get_objects(nearby_debris,box_collision);

        unordered_set<uint32_t> collisions;

        for(size_t i=0;i<nearby_debris.size();i++){
            if(!collisions.count(nearby_debris[i])){
                collisions.emplace(nearby_debris[i]);

                const Debris& debris=Game::get_debris(nearby_debris[i]);
                Collision_Rect<double> box_debris=debris.get_collision_box();

                if(Collision::check_rect(box_collision,box_debris)){
                    if(get_shot_type()->damage_type=="explosive"){
                        Game::create_explosion("explosion_missile","explosion_missile",Coords<double>(box.center_x(),box.center_y()),get_firing_upgrade()->damage);
                    }

                    die();
                }
            }
        }
    }
}

void Shot::animate(){
    if(is_alive()){
        sprite.animate();
    }
}

void Shot::render(){
    if(is_alive()){
        if(Collision::check_rect(box*Game_Manager::camera_zoom,Game_Manager::camera)){
            sprite.render(box.x*Game_Manager::camera_zoom-Game_Manager::camera.x,box.y*Game_Manager::camera_zoom-Game_Manager::camera.y,1.0,1.0,1.0,angle);

            ///QQQ render collision boxes
            /**Collision_Rect<double> col_box=get_collision_box();
            Render::render_rectangle(box.x*Game_Manager::camera_zoom-Game_Manager::camera.x,box.y*Game_Manager::camera_zoom-Game_Manager::camera.y,box.w,box.h,0.5,"ui_white");
            Render::render_rectangle(col_box.x*Game_Manager::camera_zoom-Game_Manager::camera.x,col_box.y*Game_Manager::camera_zoom-Game_Manager::camera.y,col_box.w,col_box.h,0.5,"red");*/
            ///
        }
    }
}
