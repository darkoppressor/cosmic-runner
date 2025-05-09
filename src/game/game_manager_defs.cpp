/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "game.h"
#include "background.h"
#include "../android/android_leaderboard.h"
#include "../android/android_achievements.h"

#include <game/game_manager.h>
#include <options/options.h>
#include <music/music_manager.h>
#include <screen_shake.h>
#include <engine.h>
#include <ui/font.h>
#include <data/object_manager.h>
#include <network/network_engine.h>
#include <render/render.h>
#include <game_window.h>
#include <engine_version.h>
#include <image/image_manager.h>
#include <data/data_manager.h>
#include <strings/engine_strings.h>
#include <math/engine_math.h>
#include <ui/gui_manager.h>
#include <ui/window_manager.h>

#include <ctime>

using namespace std;

void Game_Manager::on_startup () {
    if (Engine::save_data_version_is_different) {
        Window_Manager::get_window("changelog")->toggle_on();
    }

    Game::load_high_scores();

    Android_Leaderboard::remove_android_buttons();
}

bool Game_Manager::effect_allowed () {
    uint32_t effects = Game::get_effect_count();

    if (effects < Options::effect_limit) {
        return true;
    } else {
        return false;
    }
}

void Game_Manager::manage_music () {
    Android_Leaderboard::update_windows();
    Android_Achievements::update_windows();

    string music_to_play = "";

    /**if(in_progress){
        music_to_play="1";
       }
       else{
        music_to_play="";
       }*/

    if (music_to_play.length() > 0) {
        if (music_to_play != current_music) {
            Music_Manager::restart_track(music_to_play);
        }

        Music_Manager::play_track(music_to_play);
    } else {
        Music_Manager::stop_track();
    }

    current_music = music_to_play;
}

void Game_Manager::set_camera () {
    camera_delta_x = camera.x;
    camera_delta_y = camera.y;

    Screen_Shake::update_camera_before(camera);

    if (true) {
        center_camera(Game::get_player_const().get_box());
    } else {
        if (cam_state == "left") {
            camera.x -= camera_speed / (double) Engine::UPDATE_RATE;
        } else if (cam_state == "up") {
            camera.y -= camera_speed / (double) Engine::UPDATE_RATE;
        } else if (cam_state == "right") {
            camera.x += camera_speed / (double) Engine::UPDATE_RATE;
        } else if (cam_state == "down") {
            camera.y += camera_speed / (double) Engine::UPDATE_RATE;
        } else if (cam_state == "left_up") {
            camera.x -= camera_speed / (double) Engine::UPDATE_RATE;
            camera.y -= camera_speed / (double) Engine::UPDATE_RATE;
        } else if (cam_state == "right_up") {
            camera.x += camera_speed / (double) Engine::UPDATE_RATE;
            camera.y -= camera_speed / (double) Engine::UPDATE_RATE;
        } else if (cam_state == "right_down") {
            camera.x += camera_speed / (double) Engine::UPDATE_RATE;
            camera.y += camera_speed / (double) Engine::UPDATE_RATE;
        } else if (cam_state == "left_down") {
            camera.x -= camera_speed / (double) Engine::UPDATE_RATE;
            camera.y += camera_speed / (double) Engine::UPDATE_RATE;
        }
    }

    // If the camera goes past the bounds of the world, set it back in the bounds.
    if (camera.x < 0.0) {
        camera.x = 0.0;
    }

    if (camera.x + camera.w > Game::world_width* camera_zoom) {
        camera.x = Game::world_width * camera_zoom - camera.w;
    }

    if (camera.y < 0.0) {
        camera.y = 0.0;
    }

    if (camera.y + camera.h > Game::world_height* camera_zoom) {
        camera.y = Game::world_height * camera_zoom - camera.h;
    }

    Screen_Shake::update_camera_after(camera);

    camera_delta_x = camera.x - camera_delta_x;
    camera_delta_y = camera.y - camera_delta_y;
}

string Game_Manager::get_game_window_caption () {
    string msg = "";

    return msg;
}

void Game_Manager::clear_title () {
    Title& title = Game::get_title();

    title.clear_title();
}

void Game_Manager::setup_title () {
    clear_title();

    Title& title = Game::get_title();
    RNG& rng = Game::get_rng();

    rng.seed((uint32_t) time(0));

    title.setup(rng);
}

void Game_Manager::update_title_background () {
    Title& title = Game::get_title();
    RNG& rng = Game::get_rng();

    title.movement();
    title.check_ships(rng);
    title.animate(rng);
}

void Game_Manager::render_title_background () {
    Title& title = Game::get_title();

    title.render();

    if (!GUI_Manager::hide_gui) {
        Bitmap_Font* font = Object_Manager::get_font("large");

        font->show(0.0, Game_Window::height() - font->spacing_y * 2.0,
                   "Version: " + Engine_Version::get_version() + " " + Engine_Version::get_status() + "\nChecksum: " +
                   Engine::CHECKSUM, "ui_0");

        Image_Data* title_image = Image_Manager::get_image("title/title");

        Render::render_texture(Game_Window::width() / 2.0 - title_image->w / 2.0, 24.0, title_image);
    }
}

void Game_Manager::render_scoreboard () {
    if (display_scoreboard) {
        Bitmap_Font* font = Object_Manager::get_font("standard");
        string name_list = Network_Engine::get_name_list();
        string ping_list = Network_Engine::get_ping_list();

        Render::render_rectangle(0.0, 0.0, Game_Window::width(), Game_Window::height(), 0.5, "ui_black");

        font->show(72.0, (Game_Window::height() - (Strings::newline_count(name_list) + 1) * font->spacing_y) / 2.0,
                   name_list, "ui_white");
        font->show(168.0, (Game_Window::height() - (Strings::newline_count(ping_list) + 1) * font->spacing_y) / 2.0,
                   ping_list, "ui_white");
    }
}

void Game_Manager::render_pause () {
    Bitmap_Font* font = Object_Manager::get_font("large");
    string msg = "Paused";

    font->show((Game_Window::width() - (font->spacing_x * msg.length())) / 2.0,
               (Game_Window::height() - font->spacing_y) / 2.0, msg, "ui_0");
}

void Game_Manager::render_fps (int render_rate, double ms_per_frame, int logic_frame_rate) {
    Object_Manager::get_font("large")->show(2.0, 2.0,
                                            "FPS: " + Strings::num_to_string(render_rate) + "\n" +
                                            Network_Engine::get_stats(), "ui_0");
}

void Game_Manager::render_loading_screen (const Progress_Bar& bar, string message) {
    if (Game_Window::is_initialized()) {
        Game_Window::clear_renderer(Color(0, 0, 0, 255));

        if (Data_Manager::are_images_loaded()) {
            if (Game::is_generating_world()) {
                Render::render_texture(0, 0, Image_Manager::get_image("loading_background_world_generation"));
            } else {
                Render::render_texture(0, 0, Image_Manager::get_image("loading_background_game_start"));
            }
        } else if (Data_Manager::are_colors_loaded()) {
            Render::render_rectangle(0, 0, Game_Window::width(), Game_Window::height(), 1.0, "ui_2");
        }

        if (Data_Manager::are_colors_loaded()) {
            double screen_width = Game_Window::width();
            double screen_height = Game_Window::height();
            double percentage = bar.get_percentage_done();
            double max_bar_width = screen_width / 5.0;
            double bar_width = max_bar_width * percentage;
            double bar_height = screen_height / 24.0;
            double back_thickness = Math::ceil(screen_width / 640.0);
            double offset = bar_height * 2.0 + screen_height / 32.0;

            Render::render_rectangle(screen_width / 2.0 - max_bar_width / 2.0 - back_thickness,
                                     screen_height - offset - back_thickness, max_bar_width + back_thickness * 2.0,
                                     bar_height + back_thickness * 2.0, 1.0, "ui_3");
            Render::render_rectangle(screen_width / 2.0 - max_bar_width / 2.0, screen_height - offset, bar_width,
                                     bar_height, 1.0, "ui_1");

            if (Data_Manager::are_fonts_loaded()) {
                Bitmap_Font* font = Object_Manager::get_font("standard");
                string msg = Strings::num_to_string((int) (percentage * 100.0)) + "%";

                font->show((screen_width - (double) msg.length() * (double) font->spacing_x) / 2.0,
                           screen_height - offset + 6, msg, "ui_0");
                font->show((screen_width - (double) message.length() * (double) font->spacing_x) / 2.0,
                           screen_height - offset + back_thickness * 2.0 + font->spacing_y * 1.5, message, "ui_0");
            }
        }

        Game_Window::render_present();
    }
}
