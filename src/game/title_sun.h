/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef title_sun_h
#define title_sun_h

#include <collision/collision.h>
#include <collision/coords.h>
#include <rng/rng.h>
#include <image/image_data.h>

#include <SDL.h>

#include <string>
#include <vector>
#include <cstdint>

class Title_Sun_Shimmer {
    private:
        double y;
        double y_right_mod;
        bool left_side;
        bool right_side;
        double speed;
        std::int32_t radius;

    public:
        Title_Sun_Shimmer (double new_y, double new_y_right_mod, bool new_left_side, bool new_right_side,
                           double new_speed, std::int32_t new_radius);

        double get_y() const;

        bool is_done(double sun_height) const;

        void movement();

        void draw_circle(SDL_Surface* surface, std::int32_t x_coord, std::int32_t y_coord) const;

        void draw_left(SDL_Surface* surface) const;
        void draw_right(SDL_Surface* surface) const;

        void draw(SDL_Surface* surface) const;
};

class Title_Sun {
    private:
        Collision_Rect<double> box;
        SDL_Surface* base_surface;
        Image_Data image;
        std::vector<Title_Sun_Shimmer> shimmers;
        std::uint32_t shimmer_spawn;

    public:
        Title_Sun (const Coords<double>& position);

        ~Title_Sun ();

        void setup(std::string sprite_name);

        void animate(RNG& rng);
        void render();
};

#endif
