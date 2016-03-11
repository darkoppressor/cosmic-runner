/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "hud.h"
#include "game.h"
#include "game_constants.h"
#include "game_data.h"

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

void Hud::render_bar(string font_name,string font_color,string bar_color,const Coords<double>& position,double a,double b,bool show_background){
    Bitmap_Font* font=0;

    if(font_name.length()>0){
        font=Object_Manager::get_font(font_name);
    }

    if(b==0.0){
        b=1.0;
    }
    double percentage=a/b;

    double bar_width=Game_Constants::HUD_MAX_BAR_WIDTH*percentage;

    if(show_background){
        Render::render_texture(position.x,position.y,Image_Manager::get_image("hud_bar_background"));
    }

    Render::render_rectangle(position.x+Game_Constants::HUD_BAR_OFFSET_X,position.y+Game_Constants::HUD_BAR_OFFSET_Y,bar_width,Game_Constants::HUD_BAR_HEIGHT,1.0,bar_color);

    string msg=Strings::num_to_string((int32_t)a)+"/"+Strings::num_to_string((int32_t)b);

    if(font!=0){
        font->show(position.x+Game_Constants::HUD_BAR_OFFSET_X+bar_width/2.0-(msg.length()*font->spacing_x)/2.0,position.y+Game_Constants::HUD_BAR_OFFSET_Y+Game_Constants::HUD_BAR_HEIGHT/2.0-font->spacing_y/3.0,msg,font_color);
    }
}

void Hud::render(){
    const Ship& player=Game::get_player_const();

    uint32_t bar_number=0;

    render_bar("","ui_white","hud_hull",Coords<double>(Game_Constants::HUD_SPACING,Game_Constants::HUD_SPACING+Game_Constants::HUD_SPACING*(double)bar_number+Game_Constants::HUD_BAR_HEIGHT*(double)bar_number++),player.get_hull(),player.get_hull_max(),true);

    if(player.get_shields_max()>0){
        render_bar("","ui_white","hud_shields",Coords<double>(Game_Constants::HUD_SPACING,Game_Constants::HUD_SPACING+Game_Constants::HUD_SPACING*(double)bar_number+Game_Constants::HUD_BAR_HEIGHT*(double)bar_number++),player.get_shields(),player.get_shields_max(),true);
    }

    render_bar("","ui_white","hud_power",Coords<double>(Game_Constants::HUD_SPACING,Game_Constants::HUD_SPACING+Game_Constants::HUD_SPACING*(double)bar_number+Game_Constants::HUD_BAR_HEIGHT*(double)bar_number++),Game::get_power(),Game_Constants::MAX_POWER*Engine::UPDATE_RATE,true);

    Bitmap_Font* font=Object_Manager::get_font("standard");

    string msg="Score: "+Strings::num_to_string(Game::get_score())+"\nx"+Strings::num_to_string(Game::get_score_multiplier());

    font->show((double)Game_Window::width()-Game_Constants::HUD_SPACING-Strings::longest_line(msg)*font->spacing_x,Game_Constants::HUD_SPACING,msg,"ui_white");

    msg="Kills: "+Strings::num_to_string(Game::get_kills());
    double kills_x=(double)Game_Window::width()-Game_Constants::HUD_SPACING-Strings::longest_line(msg)*font->spacing_x;

    font->show(kills_x,Game_Constants::HUD_SPACING*2.0+font->spacing_y*2.0,msg,"ui_white");

    double notoriety_y=Game_Constants::HUD_SPACING*3.0+font->spacing_y*3.0;

    uint32_t notoriety_tiers=0;

    if(Game::notoriety_tier_2()){
        notoriety_tiers=2;
    }
    else if(Game::notoriety_tier_1()){
        notoriety_tiers=1;
    }

    for(uint32_t i=0;i<notoriety_tiers;i++){
        notoriety_sprite.render(kills_x+(double)i*Game_Constants::HUD_SPACING+(double)i*notoriety_sprite.get_width(),notoriety_y);
    }

    if(notoriety_tiers>0){
        if(notoriety_tiers==2){
            msg=Strings::num_to_string(Game::get_notoriety()/Engine::UPDATE_RATE-Game_Constants::NOTORIETY_TIER_2+1);
        }
        else{
            msg=Strings::num_to_string(Game::get_notoriety()/Engine::UPDATE_RATE-Game_Constants::NOTORIETY_TIER_1+1);
        }

        double tiers_width=((double)notoriety_tiers-1.0)*Game_Constants::HUD_SPACING+(double)notoriety_tiers*notoriety_sprite.get_width();

        font->show(kills_x+tiers_width/2.0-(msg.length()*font->spacing_x)/2.0,notoriety_y+Game_Constants::HUD_SPACING+notoriety_sprite.get_height(),msg,"ui_white");
    }

    if(player.has_weapon()){
        string font_color="hud_weapons_on";

        Upgrade* upgrade=Game_Data::get_upgrade_type(player.get_weapon_name());

        msg=upgrade->display_name+" ";

        if(player.are_weapons_enabled()){
            msg+="ONLINE";
        }
        else{
            msg+="OFFLINE";

            font_color="hud_weapons_off";
        }

        Coords<double> position(Game_Constants::HUD_SPACING,Game_Constants::HUD_SPACING+Game_Constants::HUD_SPACING*(double)bar_number+Game_Constants::HUD_BAR_HEIGHT*(double)bar_number++);
        font->show(position.x,position.y+Game_Constants::HUD_BAR_OFFSET_Y+Game_Constants::HUD_BAR_HEIGHT/2.0-font->spacing_y/3.0,msg,font_color);

        position.x+=msg.length()*font->spacing_x;

        uint32_t cool_point=player.get_cooldown(upgrade->cooldown)*Engine::UPDATE_RATE/1000;

        render_bar("","ui_white","hud_cooldown",position,(double)cool_point-(double)player.get_weapon_cooldown(),(double)cool_point,false);
    }

    if(player.has_active()){
        string font_color="hud_active";

        string active_name=player.get_active_name();
        Upgrade* upgrade=Game_Data::get_upgrade_type(active_name);

        msg=upgrade->display_name;

        if(active_name=="cloak"){
            if(player.is_cloaked()){
                msg+=" ONLINE";

                font_color="hud_active_on";
            }
            else{
                msg+=" OFFLINE";

                font_color="hud_active_off";
            }
        }
        else if(active_name=="warp_drive"){
            if(player.is_warping()){
                msg+=" ONLINE";

                font_color="hud_active_on";
            }
            else{
                msg+=" OFFLINE";

                font_color="hud_active_off";
            }
        }

        Coords<double> position(Game_Constants::HUD_SPACING,Game_Constants::HUD_SPACING+Game_Constants::HUD_SPACING*(double)bar_number+Game_Constants::HUD_BAR_HEIGHT*(double)bar_number++);
        font->show(position.x,position.y+Game_Constants::HUD_BAR_OFFSET_Y+Game_Constants::HUD_BAR_HEIGHT/2.0-font->spacing_y/3.0,msg,font_color);

        position.x+=msg.length()*font->spacing_x;

        uint32_t cool_point=player.get_cooldown(upgrade->cooldown)*Engine::UPDATE_RATE/1000;

        render_bar("","ui_white","hud_cooldown",position,(double)cool_point-(double)player.get_active_cooldown(),(double)cool_point,false);
    }
}
