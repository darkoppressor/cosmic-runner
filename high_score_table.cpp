/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include "high_score_table.h"

#include <file_io.h>
#include <engine_strings.h>

#include <string>

#include <boost/algorithm/string.hpp>

using namespace std;

vector<High_Score> High_Score_Table::get_high_scores() const {
    return high_scores;
}

void High_Score_Table::load_high_scores () {
    high_scores.clear();

    File_IO_Load load("high_scores", false, false, false, false);

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
        high_scores.push_back(High_Score("Hubert", 200000));
        high_scores.push_back(High_Score("Sally", 100000));
        high_scores.push_back(High_Score("Jean-Luc", 75000));
        high_scores.push_back(High_Score("Sam", 50000));
        high_scores.push_back(High_Score("Freddy", 25000));
        high_scores.push_back(High_Score("Ender", 15000));
        high_scores.push_back(High_Score("Penny", 10000));
        high_scores.push_back(High_Score("Amos", 5000));
        high_scores.push_back(High_Score("Jules", 2500));
        high_scores.push_back(High_Score("Dopefish", 1000));
    }
}

void High_Score_Table::save_high_scores () {
    string data = "";

    for (const auto high_score : high_scores) {
        data += high_score.get_name() + "," + Strings::num_to_string(high_score.get_score()) + "\n";
    }

    File_IO::save_atomic("high_scores", data, false, false, false);
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
