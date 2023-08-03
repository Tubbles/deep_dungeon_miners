#include "gamepad.hpp"
#include "util.hpp"

#include <optional>

namespace gamepad {

enum {
    LEFT_X        = GLFW_GAMEPAD_AXIS_LEFT_X,
    LEFT_Y        = GLFW_GAMEPAD_AXIS_LEFT_Y,
    RIGHT_X       = GLFW_GAMEPAD_AXIS_RIGHT_X,
    RIGHT_Y       = GLFW_GAMEPAD_AXIS_RIGHT_Y,
    LEFT_TRIGGER  = GLFW_GAMEPAD_AXIS_LEFT_TRIGGER,
    RIGHT_TRIGGER = GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER,
    BUTTON_A      = GLFW_GAMEPAD_BUTTON_A,
    BUTTON_B      = GLFW_GAMEPAD_BUTTON_B,
    BUTTON_X      = GLFW_GAMEPAD_BUTTON_X,
    BUTTON_Y      = GLFW_GAMEPAD_BUTTON_Y,
    LEFT_BUMPER   = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER,
    RIGHT_BUMPER  = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,
    BACK          = GLFW_GAMEPAD_BUTTON_BACK,
    START         = GLFW_GAMEPAD_BUTTON_START,
    GUIDE         = GLFW_GAMEPAD_BUTTON_GUIDE,
    LEFT_THUMB    = GLFW_GAMEPAD_BUTTON_LEFT_THUMB,
    RIGHT_THUMB   = GLFW_GAMEPAD_BUTTON_RIGHT_THUMB,
    DPAD_UP       = GLFW_GAMEPAD_BUTTON_DPAD_UP,
    DPAD_RIGHT    = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,
    DPAD_DOWN     = GLFW_GAMEPAD_BUTTON_DPAD_DOWN,
    DPAD_LEFT     = GLFW_GAMEPAD_BUTTON_DPAD_LEFT,

    RELEASE = GLFW_RELEASE,
    PRESS   = GLFW_PRESS,
    REPEAT  = GLFW_REPEAT,
};

static Instace gamepads[GLFW_JOYSTICK_LAST + 1];
static bool do_debug = false;

void setup()
{
    memset(gamepads, 0, sizeof(gamepads));
    for (size_t jid = 0; jid < ARR_LEN(gamepads); ++jid) {
        if (glfwJoystickPresent(jid)) {
            callback(jid, GLFW_CONNECTED);
        }
    }
}

void update()
{
    for (size_t jid = 0; jid < ARR_LEN(gamepads); ++jid) {
        auto& gamepad = gamepads[jid];
        if (gamepad.present) {
            auto& axes    = gamepad.state.axes;
            auto& buttons = gamepad.state.buttons;
            GLFWgamepadstate new_state;
            glfwGetGamepadState(jid, &new_state);

            // auto axes = glfwGetJoystickAxes(jid, &gamepad.num_axes);
            // auto buttons = glfwGetJoystickButtons(jid, &gamepad.num_buttons);

            for (int i = 0; i < gamepad.num_buttons; ++i) {
                if ((new_state.buttons[i] == PRESS) && ((buttons[i] == PRESS) || (buttons[i] == REPEAT))) {
                    buttons[i] = REPEAT;
                } else {
                    buttons[i] = new_state.buttons[i];
                }
            }

            // NOTE : There is a window here if the joystick becomes disconnected and the axes and buttons pointers are
            //        invalidated after the `if(present)` above where we might have a "use-after-free" bug, depending on
            //        whether we are running these things in parallel threads of execution.
            memcpy(axes, new_state.axes, sizeof(GLFWgamepadstate::axes[0]) * gamepad.num_axes);
            // memcpy(buttons, new_state.buttons, sizeof(GLFWgamepadstate::buttons[0]) * gamepad.num_buttons);

            if (do_debug) {
                fmt::print("\x1b[2J"); // clear screen
                fmt::print("Joystick #{} ({}) state:", jid, glfwGetJoystickName(jid));
                fmt::print("\n");
                fmt::print("\tleft_x = {: .1F} ", axes[LEFT_X]);
                fmt::print("\tleft_y = {: .1F} ", axes[LEFT_Y]);
                fmt::print("\n");
                fmt::print("\tright_x = {: .1F} ", axes[RIGHT_X]);
                fmt::print("\tright_y = {: .1F} ", axes[RIGHT_Y]);
                fmt::print("\n");
                fmt::print("\tleft_trigger = {: .1F} ", axes[LEFT_TRIGGER]);
                fmt::print("\tright_trigger = {: .1F} ", axes[RIGHT_TRIGGER]);
                fmt::print("\n");
                fmt::print("\ta = {} ", buttons[BUTTON_A]);
                fmt::print("\tb = {} ", buttons[BUTTON_B]);
                fmt::print("\tx = {} ", buttons[BUTTON_X]);
                fmt::print("\ty = {} ", buttons[BUTTON_Y]);
                fmt::print("\n");
                fmt::print("\tleft_bumper = {} ", buttons[LEFT_BUMPER]);
                fmt::print("\tright_bumper = {} ", buttons[RIGHT_BUMPER]);
                fmt::print("\n");
                fmt::print("\tback = {} ", buttons[BACK]);
                fmt::print("\tstart = {} ", buttons[START]);
                fmt::print("\tguide = {} ", buttons[GUIDE]);
                fmt::print("\n");
                fmt::print("\tleft_thumb = {} ", buttons[LEFT_THUMB]);
                fmt::print("\tright_thumb = {} ", buttons[RIGHT_THUMB]);
                fmt::print("\n");
                fmt::print("\tdpad_up = {} ", buttons[DPAD_UP]);
                fmt::print("\tdpad_right = {} ", buttons[DPAD_RIGHT]);
                fmt::print("\tdpad_down = {} ", buttons[DPAD_DOWN]);
                fmt::print("\tdpad_left = {} ", buttons[DPAD_LEFT]);
                fmt::print("\n");
            }

            if (buttons[GUIDE] && (buttons[START] == PRESS)) {
                do_debug ^= true;
                fmt::print("\x1b[2J\n"); // clear screen
            }
        }
    }
}

void callback(int jid, int event)
{
    if (event == GLFW_CONNECTED) {
        auto axes    = glfwGetJoystickAxes(jid, &gamepads[jid].num_axes);
        auto buttons = glfwGetJoystickButtons(jid, &gamepads[jid].num_buttons);
        // int hat_count;
        // auto hats = glfwGetJoystickHats(jid, &hat_count);
        gamepads[jid].present = true;

        dfmt(fmt::format("Joystick #{} ({}) was attached", jid, glfwGetJoystickName(jid)));
        dfmt(fmt::format("Joystick #{} has {} button(s)", jid, gamepads[jid].num_buttons));
        dfmt(fmt::format("Joystick #{} has {} axis(es)", jid, gamepads[jid].num_axes));
        // dfmt(fmt::format("Joystick #{} has {} hat(s)", jid, hat_count));
    } else if (event == GLFW_DISCONNECTED) {
        gamepads[jid].present = false;

        dfmt(fmt::format("Joystick #{} was detached", jid));
    }
}

bool check_quit()
{
    for (size_t jid = 0; jid < ARR_LEN(gamepads); ++jid) {
        if (gamepads[jid].present) {
            auto buttons = gamepads[jid].state.buttons;
            if (buttons[GUIDE] && (buttons[BUTTON_X] == PRESS))
                return true;
        }
    }
    return false;
}
}
