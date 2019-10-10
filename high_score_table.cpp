/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "high_score_table.h"
#include "game_constants.h"

#include <file_io.h>
#include <engine_strings.h>
#include <directories.h>

#include <string>

#include <boost/algorithm/string.hpp>

using namespace std;

vector<High_Score> High_Score_Table::get_high_scores () const {
    return high_scores;
}

void High_Score_Table::load_high_scores () {
    high_scores.clear();

    File_IO_Load load(Directories::get_save_directory() + "high_scores");

    if (load.is_opened()) {
        while (!load.eof()) {
            string line = "";

            load.getline(&line);
            boost::algorithm::trim(line);

            if (line.length() > 0) {
                vector<string> values;
                boost::algorithm::split(values, line, boost::algorithm::is_any_of(","));

                if (values.size() >= 2) {
                    high_scores.push_back(High_Score(values[0], Strings::string_to_unsigned_long(values[1])));
                }
            }
        }

        load.close();
    } else {
        high_scores.push_back(High_Score("Hubert", Game_Constants::DEFAULT_HIGH_SCORE_10));
        high_scores.push_back(High_Score("Sally", Game_Constants::DEFAULT_HIGH_SCORE_9));
        high_scores.push_back(High_Score("Jean-Luc", Game_Constants::DEFAULT_HIGH_SCORE_8));
        high_scores.push_back(High_Score("Sam", Game_Constants::DEFAULT_HIGH_SCORE_7));
        high_scores.push_back(High_Score("Freddy", Game_Constants::DEFAULT_HIGH_SCORE_6));
        high_scores.push_back(High_Score("Ender", Game_Constants::DEFAULT_HIGH_SCORE_5));
        high_scores.push_back(High_Score("Penny", Game_Constants::DEFAULT_HIGH_SCORE_4));
        high_scores.push_back(High_Score("Amos", Game_Constants::DEFAULT_HIGH_SCORE_3));
        high_scores.push_back(High_Score("Jules", Game_Constants::DEFAULT_HIGH_SCORE_2));
        high_scores.push_back(High_Score("Dopefish", Game_Constants::DEFAULT_HIGH_SCORE_1));
    }
}

void High_Score_Table::save_high_scores () {
    string data = "";

    for (const auto high_score : high_scores) {
        data += high_score.get_name() + "," + Strings::num_to_string(high_score.get_score()) + "\n";
    }

    File_IO::save_atomic(Directories::get_save_directory() + "high_scores", data);
}

bool High_Score_Table::is_score_high (uint64_t score) {
    for (const auto high_score : high_scores) {
        if (score > high_score.get_score()) {
            return true;
        }
    }

    return false;
}

void High_Score_Table::add_score (const High_Score new_score) {
    if (is_score_high(new_score.get_score())) {
        for (size_t i = 0; i < high_scores.size(); i++) {
            High_Score high_score = high_scores[i];

            if (new_score.get_score() > high_score.get_score()) {
                high_scores.insert(high_scores.begin() + i, new_score);

                high_scores.pop_back();

                break;
            }
        }
    }
}
