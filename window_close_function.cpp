/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include <window.h>
#include <log.h>
#include <engine_input.h>
#include <object_manager.h>

using namespace std;

void Window::exec_close_function () {
    if (close_function.length() > 0) {
        if (close_function == "configure_command") {
            Object_Manager::configure_command = -1;
        } else {
            Log::add_error("Invalid close function: '" + close_function + "'");
        }
    }
}
