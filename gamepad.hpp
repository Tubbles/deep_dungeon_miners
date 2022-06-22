#pragma once

#include <GLFW/glfw3.h>

namespace gamepad {
struct Instace {
    bool present = false;
    GLFWgamepadstate state;
    int num_buttons;
    int num_axes;
};

void setup();
void update();
void callback(int jid, int event);
bool check_quit();
}