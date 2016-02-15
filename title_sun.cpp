/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "title_sun.h"

#include <render.h>
#include <engine.h>
#include <image_manager.h>

using namespace std;

Title_Sun::Title_Sun(string new_sprite,const Coords<double>& position){
    box.x=position.x;
    box.y=position.y;

    sprite.set_name(new_sprite);

    box.w=sprite.get_width();
    box.h=sprite.get_height();

    shimmer_spawn=0;
}

void Title_Sun::animate(RNG& rng){
    sprite.animate();

    for(size_t i=0;i<shimmers.size();){
        shimmers[i]+=0.25;

        if(shimmers[i]>=sprite.get_height()){
            shimmers.erase(shimmers.begin()+i);
        }
        else{
            i++;
        }
    }

    if(++shimmer_spawn>=uint32_t(0.75*(double)Engine::UPDATE_RATE)){
        shimmer_spawn=0;

        if(shimmers.size()<6 && rng.random_range(0,99)<50){
            shimmers.push_back(0.0);
        }
    }
}

void Title_Sun::render(){
    sprite.render(box.x,box.y);

    for(size_t i=0;i<shimmers.size();i++){
        double scale=shimmers[i]/(sprite.get_height()/2.0);

        if(shimmers[i]>sprite.get_height()/2.0){
            scale=2.0-scale;
        }

        scale+=0.40;

        if(scale>1.0){
            scale=1.0;
        }

        Image_Data* shimmer=Image_Manager::get_image("title_sun_shimmer");

        double x=box.x+box.w/2.0-(shimmer->w*scale)/2.0;

        Render::render_texture(x,box.y+shimmers[i],shimmer,1.0,scale,1.0);
    }
}
