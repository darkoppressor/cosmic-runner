/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "game.h"
#include "game_constants.h"

#include <game_manager.h>
#include <network_engine.h>
#include <network_server.h>
#include <engine.h>
#include <object_manager.h>
#include <options.h>
#include <engine_input.h>
#include <window_manager.h>
#include <engine_strings.h>
#include <sound_manager.h>

using namespace std;

void Game_Manager::handle_drag_and_drop(string file){
    ///Do something with file
}

void Game_Manager::prepare_for_input(){
    if(in_progress){
        command_states.clear();

        display_scoreboard=false;
    }
}

void Game_Manager::handle_command_states_multiplayer(){
    if(in_progress){
        if(Network_Engine::status=="server"){
            Network_Server::prepare_server_input_states();

            for(size_t i=0;i<Network_Engine::clients.size();i++){
                if(!paused){
                    //Example multiplayer command state
                    /**if(Network_Engine::clients[i].game_command_state("some_command")){
                        ///Deal with command state here
                    }*/
                }
            }
        }
    }
}

void Game_Manager::handle_game_commands_multiplayer(){
    if(in_progress){
        if(Network_Engine::status=="server"){
            for(size_t i=0;i<Network_Engine::clients.size();i++){
                for(size_t j=0;j<Network_Engine::clients[i].command_buffer.size();j++){
                    string command_name=Network_Engine::clients[i].command_buffer[j];

                    if(!paused){
                        //Example multiplayer command
                        /**if(command_name=="some_command"){
                            ///Deal with command here
                        }*/
                    }
                }

                Network_Engine::clients[i].command_buffer.clear();
            }
        }
    }
}

void Game_Manager::handle_input_states_gui(){
    int mouse_x=0;
    int mouse_y=0;
    Engine::get_mouse_state(&mouse_x,&mouse_y);

    if(in_progress){
        if(Object_Manager::game_command_state("scoreboard")){
            display_scoreboard=true;
        }
    }
}

void Game_Manager::handle_input_states(){
    int mouse_x=0;
    int mouse_y=0;
    Engine::get_mouse_state(&mouse_x,&mouse_y);

    if(in_progress){
        if(!paused){
            //Example multiplayer command state
            /**if(Object_Manager::game_command_state("some_command")){
                command_states.push_back("some_command");
            }*/

            //Move the ship via directional keys
            if(Game::move_input_state("left")){
                Game::player_thrust("left");
            }
            if(Game::move_input_state("up")){
                Game::player_thrust("up");
            }
            if(Game::move_input_state("right")){
                Game::player_thrust("right");
            }
            if(Game::move_input_state("down")){
                Game::player_thrust("down");
            }
            if(Game::move_input_state("left") && Game::move_input_state("up")){
                Game::player_thrust("left_up");
            }
            if(Game::move_input_state("up") && Game::move_input_state("right")){
                Game::player_thrust("right_up");
            }
            if(Game::move_input_state("right") && Game::move_input_state("down")){
                Game::player_thrust("right_down");
            }
            if(Game::move_input_state("down") && Game::move_input_state("left")){
                Game::player_thrust("left_down");
            }
            if(Game::move_input_state("left") && Game::move_input_state("right")){
                Game::player_thrust("left");
            }
            if(Game::move_input_state("up") && Game::move_input_state("down")){
                Game::player_thrust("up");
            }
            if(Game::move_input_state("left") && Game::move_input_state("up") && Game::move_input_state("right")){
                Game::player_thrust("left_up");
            }
            if(Game::move_input_state("left") && Game::move_input_state("down") && Game::move_input_state("right")){
                Game::player_thrust("left_down");
            }
            if(Game::move_input_state("left") && Game::move_input_state("up") && Game::move_input_state("down")){
                Game::player_thrust("left_up");
            }
            if(Game::move_input_state("up") && Game::move_input_state("right") && Game::move_input_state("down")){
                Game::player_thrust("right_up");
            }
            if(Game::move_input_state("left") && Game::move_input_state("up") && Game::move_input_state("right") && Game::move_input_state("down")){
                Game::player_thrust("left_up");
            }
            if(!Game::move_input_state("left") && !Game::move_input_state("up") && !Game::move_input_state("right") && !Game::move_input_state("down")){
                Game::player_thrust("none");
            }

            Game::player_brake(Object_Manager::game_command_state("brake"));
        }
    }
}

bool Game_Manager::handle_game_command_gui(string command_name){
    //Pause the game
    if(command_name=="pause"){
        if(!Window_Manager::is_window_open(Window_Manager::get_window("game_over")) &&
           !Game::player_is_landed()){
            toggle_pause();
        }

        return true;
    }

    //Example multiplayer pause
    /**if(command_name=="pause"){
        if(Network_Engine::status=="server"){
            toggle_pause();

            Network_Server::send_paused();
        }

        return true;
    }*/

    //Toggle chat box
    else if(command_name=="chat"){
        Engine::chat.toggle_on();

        return true;
    }

    return false;
}

bool Game_Manager::handle_game_command(string command_name){
    const uint8_t* keystates=SDL_GetKeyboardState(NULL);

    ///DEV COMMANDS
    if(Options::dev && keystates[SDL_SCANCODE_F1]){
        //Example dev command
        /**if(command_name=="some_dev_command"){
            ///Dev command here.

            return true;
        }*/
    }
    ///END OF DEV COMMANDS

    if(!paused){
        if(command_name=="land"){
            if(!Game::player_is_landing() && !Game::player_is_landed()){
                const Ship& player=Game::get_player_const();

                if(Game::player_has_contract()){
                    const Planet& planet=Game::get_contract_planet();

                    if(Collision::check_circ_rect(planet.get_circle(),player.get_box())){
                        Game::commence_landing(Game::get_contract_planet_index());
                    }
                }
                else{
                    uint32_t nearest_planet_index=Game::get_nearest_planet();

                    const Planet& planet=Game::get_planet(nearest_planet_index);

                    if(Collision::check_circ_rect(planet.get_circle(),player.get_box())){
                        Game::commence_landing(nearest_planet_index);
                    }
                }
            }

            return true;
        }
        else if(command_name=="drop_cargo"){
            if(Game::player_has_contract()){
                Game::cancel_contract();

                for(uint32_t i=0;i<5;i++){
                    const Ship& player=Game::get_player_const();

                    Game::create_effect("effect_cargo_"+Strings::num_to_string(Game::get_rng().random_range(0,0)),1.0,
                                        Coords<double>(player.get_box().center_x(),player.get_box().center_y()),"",
                                        Vector(Game::get_rng().random_range(0,10),Game::get_rng().random_range(0,359)),
                                        Game::get_rng().random_range(0,359),Vector(0.01*Game::get_rng().random_range(0,50),Game::get_rng().random_range(0,359)),Game_Constants::EFFECT_LENGTH_CARGO);
                }

                Sound_Manager::play_sound("drop_cargo");
            }
            else{
                Sound_Manager::play_sound("cannot_drop_cargo");
            }

            return true;
        }

        //Example multiplayer command input
        /**if(command_name=="some_command"){
            Network_Engine::add_command(command_name);

            return true;
        }*/
    }

    return false;
}

bool Game_Manager::handle_input_events_gui(){
    bool event_consumed=false;

    if(in_progress){
        const vector<Game_Command>& game_commands=Object_Manager::get_game_commands();

        for(size_t i=0;i<game_commands.size() && !event_consumed;i++){
            if((Engine_Input::event.type==SDL_CONTROLLERBUTTONDOWN && game_commands[i].button==Engine_Input::event.cbutton.button) ||
               (Engine_Input::event.type==SDL_KEYDOWN && Engine_Input::event.key.repeat==0 && game_commands[i].key==Engine_Input::event.key.keysym.scancode)){
                event_consumed=handle_game_command_gui(game_commands[i].name);
            }
        }
    }

    return event_consumed;
}

bool Game_Manager::handle_input_events(){
    bool event_consumed=false;

    if(in_progress){
        const vector<Game_Command>& game_commands=Object_Manager::get_game_commands();

        for(size_t i=0;i<game_commands.size() && !event_consumed;i++){
            if((Engine_Input::event.type==SDL_CONTROLLERBUTTONDOWN && game_commands[i].button==Engine_Input::event.cbutton.button) ||
               (Engine_Input::event.type==SDL_KEYDOWN && Engine_Input::event.key.repeat==0 && game_commands[i].key==Engine_Input::event.key.keysym.scancode)){
                event_consumed=handle_game_command(game_commands[i].name);
            }
        }
    }

    return event_consumed;
}
