/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include "high_score.h"

using namespace std;

High_Score::High_Score (string name, uint64_t score) {
    this->name = name;
    this->score = score;
}

string High_Score::get_name () const {
    return name;
}

uint64_t High_Score::get_score () const {
    return score;
}
