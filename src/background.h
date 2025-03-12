/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#ifndef background_h
#define background_h

#include <image_data.h>
#include <rng.h>

#include <string>
#include <vector>

class Background_Layer {
    public:
        // Every layer uses two identical images

        // The x and y positions of the first layer image
        double position_x_a;
        double position_y_a;

        // The x and y positions of the second layer image
        double position_x_b;
        double position_y_b;

        // The dimensions of layer images
        double width;
        double height;

        // The rate at which the background scrolls
        // Lower numbers scroll the background faster
        double parallax_speed_x, parallax_speed_y;
        Image_Data layer_image;

        Background_Layer (double get_width, double get_height, double get_parallax_speed_x,
                          double get_parallax_speed_y);

        void generate_texture_background(std::string type, int layer_number, RNG& rng);

        void update(double camera_delta_x, double camera_delta_y);

        void render(double opacity);
};

class Background {
    private:
        std::vector<Background_Layer> background_layers;

    public:
        Background ();

        void setup(std::string type, RNG& rng);

        void unload();

        void update(double camera_delta_x, double camera_delta_y);

        void render(double opacity);
};

#endif
