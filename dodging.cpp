/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "dodging.h"
#include "game.h"
#include "game_constants.h"

#include <engine.h>

using namespace std;

void Dodging::clear_lists(){
    tracking.clear();
    cooling.clear();
}

vector<uint32_t> Dodging::get_tracking() const{
    return tracking;
}

bool Dodging::is_debris_tracked(uint32_t index) const{
    for(size_t i=0;i<tracking.size();i++){
        if(tracking[i]==index){
            return true;
        }
    }

    return false;
}

bool Dodging::is_debris_cooling(uint32_t index) const{
    return cooling.count(index);
}

void Dodging::begin_tracking_debris(uint32_t index){
    if(!is_debris_tracked(index)){
        tracking.push_back(index);
    }
}

void Dodging::begin_cooling_debris(uint32_t index){
    cooling.emplace(index,Game_Constants::DODGE_OBJECT_COOLDOWN*Engine::UPDATE_RATE/1000);
}

void Dodging::stop_tracking_debris(uint32_t index){
    for(size_t i=0;i<tracking.size();){
        if(tracking[i]==index){
            tracking.erase(tracking.begin()+i);
        }
        else{
            i++;
        }
    }
}

void Dodging::cooldown(){
    for(auto it=cooling.begin();it!=cooling.end();){
        if(--it->second==0){
            it=cooling.erase(it);
        }
        else{
            it++;
        }
    }
}
