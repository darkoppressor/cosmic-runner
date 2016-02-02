/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "title.h"

#include <engine.h>

#include <ctime>

using namespace std;

Title::Title(){
    seed=0;
}

void Title::reseed(){
    seed=(uint32_t)time(0);
}

uint32_t Title::get_seed() const{
    return seed;
}

void Title::accelerate(RNG& rng){
    if(rng.random_range(0,999)<5){
        velocity.magnitude=0.01*rng.random_range(0,20000);
        velocity.direction=rng.random_range(0,359);
    }
}

Coords<double> Title::movement(){
    Vector_Components vc=velocity.get_components();

    Coords<double> change;

    change.x=vc.a/(double)Engine::UPDATE_RATE;
    change.y=vc.b/(double)Engine::UPDATE_RATE;

    return change;
}
