/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include <engine_version.h>

using namespace std;

string Engine_Version::get_year () {
    return "2025"; // YEAR
}

string Engine_Version::get_month () {
    return "02"; // MONTH
}

string Engine_Version::get_day () {
    return "24"; // DAY
}

int Engine_Version::get_major () {
    return 0; // MAJOR
}

int Engine_Version::get_minor () {
    return 3; // MINOR
}

int Engine_Version::get_micro () {
    return 2; // MICRO
}

string Engine_Version::get_status () {
    return "beta"; // STATUS
}

void Engine_Version::populate_version_series (vector<Version_Series>& version_series) {
    version_series.push_back(Version_Series("0.0.1", "0.0.1"));
    version_series.push_back(Version_Series("0.1.0", "0.3.2"));
}
