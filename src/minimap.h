/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef minimap_h
#define minimap_h

#include <image_data.h>
#include <collision.h>

#include <cstdint>

class Minimap {
    private:
        Image_Data image_data;
        std::uint32_t width;
        std::uint32_t height;

        bool is_generated() const;

        double get_scale_x() const;
        double get_scale_y() const;

    public:
        Minimap ();

        void clear_map();

        void generate_map(std::uint32_t new_width, std::uint32_t new_height);

        Collision_Rect<double> get_box() const;

        void render();
};

#endif
