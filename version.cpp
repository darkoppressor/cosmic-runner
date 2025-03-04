/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include <engine_version.h>

using namespace std;

string Engine_Version::get_year () {
    return "2025"; // YEAR
}

string Engine_Version::get_month () {
    return "02"; // MONTH
}

string Engine_Version::get_day () {
    return "25"; // DAY
}

int Engine_Version::get_major () {
    return 0; // MAJOR
}

int Engine_Version::get_minor () {
    return 3; // MINOR
}

int Engine_Version::get_patch () {
    return 2; // PATCH
}

string Engine_Version::get_status () {
    return "beta"; // STATUS
}

void Engine_Version::populate_version_series (vector<Version_Series>& version_series) {
    version_series.push_back(Version_Series("0.0.1", "0.0.1"));
    version_series.push_back(Version_Series("0.1.0", "0.3.2"));
}
