#pragma once

#include "util.hpp"
#include <SFML/Window/Joystick.hpp>

namespace gamepad {
struct Instance {
    int id;
    bool present = false;
    unsigned char buttons[sf::Joystick::ButtonCount];
    float axes[sf::Joystick::AxisCount];
    int num_buttons;
    int num_axes;
    int player;

    void init(int id) {
        this->id = id;
        this->present = false;
        ARR_ZERO(this->buttons);
        ARR_ZERO(this->axes);
        this->num_buttons = 0;
        this->num_axes = 0;
        this->player = 0;
    }
};

void setup();
// void update();
// void callback(int jid, int event);
// bool check_quit();
} // namespace gamepad
