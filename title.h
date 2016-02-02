/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef title_h
#define title_h

#include <math_vector.h>
#include <rng.h>
#include <coords.h>

#include <cstdint>

class Title{
private:

    //The seed for the game-wide RNG is stored here
    //Each time the title is setup, this is changed using the current time
    std::uint32_t seed;

public:

    Vector velocity;

    Title();

    void reseed();
    std::uint32_t get_seed() const;

    void accelerate(RNG& rng);
    //Returns the change in x and y as coords
    Coords<double> movement();
};

#endif
