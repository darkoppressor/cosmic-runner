/* Copyright (c) 2012 Cheese and Bacon Games, LLC */
/* This file is licensed under the MIT License. */
/* See the file docs/LICENSE.txt for the full license text. */

#include <engine_version.h>

using namespace std;

string Engine_Version::get_year(){
    return "2018"; //YEAR
}

string Engine_Version::get_month(){
    return "07"; //MONTH
}

string Engine_Version::get_day(){
    return "05"; //DAY
}

int Engine_Version::get_major(){
    return 0; //MAJOR
}

int Engine_Version::get_minor(){
    return 2; //MINOR
}

int Engine_Version::get_micro(){
    return 3; //MICRO
}

string Engine_Version::get_status(){
    return "beta"; //STATUS
}

void Engine_Version::populate_version_series(vector<Version_Series>& version_series){
    version_series.push_back(Version_Series("0.0.1","0.0.1"));
    version_series.push_back(Version_Series("0.1.0","0.2.3"));
}
