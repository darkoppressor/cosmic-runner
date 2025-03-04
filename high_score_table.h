/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

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
