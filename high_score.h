/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef high_score_h
#define high_score_h

#include <string>
#include <cstdint>

class High_Score {
private:

    std::string name;
    std::uint64_t score;

public:

    High_Score(std::string name, std::uint64_t score);

    std::string get_name() const;
    std::uint64_t get_score() const;
};

#endif
