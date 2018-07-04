/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#ifndef high_score_table_h
#define high_score_table_h

#include "high_score.h"

#include <vector>
#include <cstdint>

class High_Score_Table {
private:

    std::vector<High_Score> high_scores;

public:

    std::vector<High_Score> get_high_scores() const;

    void load_high_scores();
    void save_high_scores();

    bool is_score_high(std::uint64_t score);
    void add_score(const High_Score new_score);
};

#endif
