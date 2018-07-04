/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

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
