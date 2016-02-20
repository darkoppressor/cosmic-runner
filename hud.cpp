/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "hud.h"
#include "game.h"
#include "game_constants.h"

#include <font.h>
#include <object_manager.h>
#include <render.h>
#include <image_manager.h>
#include <engine_strings.h>
#include <engine.h>
#include <game_window.h>

using namespace std;

Sprite Hud::notoriety_sprite;

void Hud::setup(){
    notoriety_sprite.set_name("hud_notoriety_tier");
}

void Hud::animate(){
    notoriety_sprite.animate();
}

void Hud::render_bar(string font_name,string font_color,string bar_color,const Coords<double>& position,double a,double b){
    ///QQQ constants
    double HUD_MAX_BAR_WIDTH=128.0;
    double HUD_BAR_HEIGHT=24.0;
    double HUD_BAR_OFFSET_X=14.0;
    double HUD_BAR_OFFSET_Y=7.0;

    Bitmap_Font* font=Object_Manager::get_font(font_name);

    if(b==0.0){
        b=1.0;
    }
    double percentage=a/b;

    double bar_width=HUD_MAX_BAR_WIDTH*percentage;

    Render::render_texture(position.x,position.y,Image_Manager::get_image("hud_bar_background"));

    Render::render_rectangle(position.x+HUD_BAR_OFFSET_X,position.y+HUD_BAR_OFFSET_Y,bar_width,HUD_BAR_HEIGHT,1.0,bar_color);

    string msg=Strings::num_to_string((int32_t)a)+"/"+Strings::num_to_string((int32_t)b);

    ///font->show(position.x+HUD_BAR_OFFSET_X+bar_width/2.0-(msg.length()*font->spacing_x)/2.0,position.y+HUD_BAR_OFFSET_Y+HUD_BAR_HEIGHT/2.0-font->spacing_y/3.0,msg,font_color);
}

void Hud::render(){
    ///QQQ constants
    double HUD_SPACING=24.0;
    ///repeat
    double HUD_BAR_HEIGHT=24.0;

    uint32_t bar_number=0;

    render_bar("standard","ui_white","hud_hull",Coords<double>(HUD_SPACING,HUD_SPACING+HUD_SPACING*(double)bar_number+HUD_BAR_HEIGHT*(double)bar_number++),Game::get_player_const().get_hull(),Game::get_player_const().get_hull_max());

    if(Game::get_player_const().get_shields_max()>0){
        render_bar("standard","ui_white","hud_shields",Coords<double>(HUD_SPACING,HUD_SPACING+HUD_SPACING*(double)bar_number+HUD_BAR_HEIGHT*(double)bar_number++),Game::get_player_const().get_shields(),Game::get_player_const().get_shields_max());
    }

    render_bar("standard","ui_white","hud_power",Coords<double>(HUD_SPACING,HUD_SPACING+HUD_SPACING*(double)bar_number+HUD_BAR_HEIGHT*(double)bar_number++),Game::get_power(),Game_Constants::MAX_POWER*Engine::UPDATE_RATE);

    Bitmap_Font* font=Object_Manager::get_font("standard");

    string msg="Score: "+Strings::num_to_string(Game::get_score())+"\nx"+Strings::num_to_string(Game::get_score_multiplier());

    font->show((double)Game_Window::width()-HUD_SPACING-Strings::longest_line(msg)*font->spacing_x,HUD_SPACING,msg,"ui_white");

    msg="Kills: "+Strings::num_to_string(Game::get_kills());
    double kills_x=(double)Game_Window::width()-HUD_SPACING-Strings::longest_line(msg)*font->spacing_x;

    font->show(kills_x,HUD_SPACING*2.0+font->spacing_y*2.0,msg,"ui_white");

    double notoriety_y=HUD_SPACING*3.0+font->spacing_y*3.0;

    uint32_t notoriety_tiers=0;

    if(Game::notoriety_tier_2()){
        notoriety_tiers=2;
    }
    else if(Game::notoriety_tier_1()){
        notoriety_tiers=1;
    }

    for(uint32_t i=0;i<notoriety_tiers;i++){
        notoriety_sprite.render(kills_x+(double)i*HUD_SPACING+(double)i*notoriety_sprite.get_width(),notoriety_y);
    }

    if(notoriety_tiers>0){
        if(notoriety_tiers==2){
            msg=Strings::num_to_string(Game::get_notoriety()/Engine::UPDATE_RATE-Game_Constants::NOTORIETY_TIER_2+1);
        }
        else{
            msg=Strings::num_to_string(Game::get_notoriety()/Engine::UPDATE_RATE-Game_Constants::NOTORIETY_TIER_1+1);
        }

        double tiers_width=((double)notoriety_tiers-1.0)*HUD_SPACING+(double)notoriety_tiers*notoriety_sprite.get_width();

        font->show(kills_x+tiers_width/2.0-(msg.length()*font->spacing_x)/2.0,notoriety_y+HUD_SPACING+notoriety_sprite.get_height(),msg,"ui_white");
    }

    /**
    if(Game::get_player_const().are_weapons_enabled()){
        msg+="Weapons Systems: Enabled\n";
    }
    else{
        msg+="Weapons Systems: Disabled\n";
    }
    */
}
