/* Copyright (c) Cheese and Bacon Games, LLC. See docs/LICENSE.md for details. */

#include <ui/window.h>
#include <log.h>
#include <ui/engine_input.h>
#include <data/object_manager.h>

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
