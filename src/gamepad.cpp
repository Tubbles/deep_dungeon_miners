#include "gamepad.hpp"
#include "base.hpp"
#include "event.hpp"
#include "imgui-SFML.h"
#include "imgui.h"
#include "log.hpp"
#include "schd_event.hpp"
#include "util.hpp"
#include <optional>

namespace gamepad {

enum {
    LEFT_X = 0,
    LEFT_Y,
    LEFT_TRIGGER,
    RIGHT_TRIGGER,
    RIGHT_X,
    RIGHT_Y,
    DPAD_HOR,
    DPAD_VER,

    BUTTON_A = 0,
    BUTTON_B,
    BUTTON_X,
    BUTTON_Y,
    LEFT_BUMPER,
    RIGHT_BUMPER,
    BACK,
    START,
    GUIDE,
    LEFT_THUMB,
    RIGHT_THUMB,

    RELEASE = 0,
    PRESS,
    REPEAT,
};

static void update();

static Instance gamepads[sf::Joystick::Count];
static bool do_debug = false;

// static void check_connection(Instance &instance) {
//     if (sf::Joystick::isConnected(instance.id) && !instance.present) {
//         // Gamepad connected
//         instance.present = true;
//         instance.num_buttons = sf::Joystick::isButtonPressed(instance.id);
//         instance.num_buttons = sf::Joystick::hasAxis(instance.id, sf::Joystick::Axis::);
//         auto axes    = glfwGetJoystickAxes(jid, &gamepads[jid].num_axes);
//         auto buttons = glfwGetJoystickButtons(jid, &gamepads[jid].num_buttons);
//         // int hat_count;
//         // auto hats = glfwGetJoystickHats(jid, &hat_count);

//         dfmt(fmt::format("Joystick #{} ({}) was attached", jid, glfwGetJoystickName(jid)));
//         dfmt(fmt::format("Joystick #{} has {} button(s)", jid, gamepads[jid].num_buttons));
//         dfmt(fmt::format("Joystick #{} has {} axis(es)", jid, gamepads[jid].num_axes));
//         dfmt(fmt::format("Joystick #{} has {} hat(s)", jid, hat_count));
//     } else if (!sf::Joystick::isConnected(instance.id) && instance.present) {
//         // Gamepad disconnected
//         instance.present = false;
//         dfmt(fmt::format("Joystick #{} was detached", jid));
//     }
// }

void setup() {
    event::sfml::register_callback(event::sfml::Type::JoystickConnected, PRIO_NORMAL, [&](event::sfml::Event event) {
        auto jid = event.joystickConnect.joystickId;
        auto &gamepad = gamepads[jid];
        gamepad.init(jid);

        if (gamepad.present) {
            spdlog::error("Joystick #{} was attached, but is already present", jid);
            return event::Action::Pass;
        }

        auto info = sf::Joystick::getIdentification(jid);
        gamepad.present = true;
        gamepad.num_buttons = sf::Joystick::getButtonCount(jid);
        gamepad.num_axes += sf::Joystick::hasAxis(jid, sf::Joystick::Axis::X);
        gamepad.num_axes += sf::Joystick::hasAxis(jid, sf::Joystick::Axis::Y);
        gamepad.num_axes += sf::Joystick::hasAxis(jid, sf::Joystick::Axis::Z);
        gamepad.num_axes += sf::Joystick::hasAxis(jid, sf::Joystick::Axis::R);
        gamepad.num_axes += sf::Joystick::hasAxis(jid, sf::Joystick::Axis::U);
        gamepad.num_axes += sf::Joystick::hasAxis(jid, sf::Joystick::Axis::V);
        gamepad.num_axes += sf::Joystick::hasAxis(jid, sf::Joystick::Axis::PovX);
        gamepad.num_axes += sf::Joystick::hasAxis(jid, sf::Joystick::Axis::PovY);

        spdlog::info("Joystick #{} ({} {} {}) was attached", jid, info.name.toAnsiString(), info.vendorId,
                     info.productId);
        spdlog::info("Joystick #{} has {} button(s)", jid, gamepad.num_buttons);
        spdlog::info("Joystick #{} has {} axis(es)", jid, gamepad.num_axes);

        return event::Action::Pass;
    });

    event::sfml::register_callback(event::sfml::Type::JoystickDisconnected, PRIO_NORMAL, [&](event::sfml::Event event) {
        auto jid = event.joystickConnect.joystickId;

        if (!gamepads[jid].present) {
            spdlog::error("Joystick #{} was detached, but is already not present", jid);
            return event::Action::Pass;
        }

        gamepads[jid].present = false;
        spdlog::info("Joystick #{} was detached", jid);

        return event::Action::Pass;
    });

    event::schd::register_callback(event::schd::Type::Update, PRIO_NORMAL, [&](event::schd::Event event) {
        UNUSED(event);
        // auto delta = event.update.delta;

        update();

        return event::Action::Pass;
    });

    event::schd::register_callback(event::schd::Type::Draw, PRIO_NORMAL, [&](event::schd::Event event) {
        auto &window = *event.draw.window;
        UNUSED(window);
        if (do_debug) {
            ImGui::Begin("FPS");
            for (size_t jid = 0; jid < ARR_LEN(gamepads); ++jid) {
                auto &gamepad = gamepads[jid];

                if (gamepad.present) {
                    ImGui::Text("%s", fmt::format("Joystick #{} ({}) state:", jid,
                                                  sf::Joystick::getIdentification(jid).name.toAnsiString())
                                          .c_str());
                    ImGui::Text("%s", fmt::format("\n").c_str());
                    ImGui::Text("%s", fmt::format("left_x = {: .1F} ", gamepad.axes[LEFT_X]).c_str());
                    ImGui::Text("%s", fmt::format("left_y = {: .1F} ", gamepad.axes[LEFT_Y]).c_str());
                    ImGui::Text("%s", fmt::format("\n").c_str());
                    ImGui::Text("%s", fmt::format("right_x = {: .1F} ", gamepad.axes[RIGHT_X]).c_str());
                    ImGui::Text("%s", fmt::format("right_y = {: .1F} ", gamepad.axes[RIGHT_Y]).c_str());
                    ImGui::Text("%s", fmt::format("\n").c_str());
                    ImGui::Text("%s", fmt::format("left_trigger = {: .1F} ", gamepad.axes[LEFT_TRIGGER]).c_str());
                    ImGui::Text("%s", fmt::format("right_trigger = {: .1F} ", gamepad.axes[RIGHT_TRIGGER]).c_str());
                    ImGui::Text("%s", fmt::format("\n").c_str());
                    ImGui::Text("%s", fmt::format("a = {} ", gamepad.buttons[BUTTON_A]).c_str());
                    ImGui::Text("%s", fmt::format("b = {} ", gamepad.buttons[BUTTON_B]).c_str());
                    ImGui::Text("%s", fmt::format("x = {} ", gamepad.buttons[BUTTON_X]).c_str());
                    ImGui::Text("%s", fmt::format("y = {} ", gamepad.buttons[BUTTON_Y]).c_str());
                    ImGui::Text("%s", fmt::format("\n").c_str());
                    ImGui::Text("%s", fmt::format("left_bumper = {} ", gamepad.buttons[LEFT_BUMPER]).c_str());
                    ImGui::Text("%s", fmt::format("right_bumper = {} ", gamepad.buttons[RIGHT_BUMPER]).c_str());
                    ImGui::Text("%s", fmt::format("\n").c_str());
                    ImGui::Text("%s", fmt::format("back = {} ", gamepad.buttons[BACK]).c_str());
                    ImGui::Text("%s", fmt::format("start = {} ", gamepad.buttons[START]).c_str());
                    ImGui::Text("%s", fmt::format("guide = {} ", gamepad.buttons[GUIDE]).c_str());
                    ImGui::Text("%s", fmt::format("\n").c_str());
                    ImGui::Text("%s", fmt::format("left_thumb = {} ", gamepad.buttons[LEFT_THUMB]).c_str());
                    ImGui::Text("%s", fmt::format("right_thumb = {} ", gamepad.buttons[RIGHT_THUMB]).c_str());
                    ImGui::Text("%s", fmt::format("\n").c_str());
                    ImGui::Text("%s", fmt::format("dpad_hor = {} ", gamepad.axes[DPAD_HOR]).c_str());
                    ImGui::Text("%s", fmt::format("dpad_ver = {} ", gamepad.axes[DPAD_VER]).c_str());
                    // ImGui::Text("%s", fmt::format("dpad_up = {} ", gamepad.buttons[DPAD_UP]).c_str());
                    // ImGui::Text("%s", fmt::format("dpad_right = {} ", gamepad.buttons[DPAD_RIGHT]).c_str());
                    // ImGui::Text("%s", fmt::format("dpad_down = {} ", gamepad.buttons[DPAD_DOWN]).c_str());
                    // ImGui::Text("%s", fmt::format("dpad_left = {} ", gamepad.buttons[DPAD_LEFT]).c_str());
                    ImGui::Text("%s", fmt::format("\n").c_str());
                    for (int i = 0; i < gamepad.num_buttons; i++) {
                        ImGui::Text("%s", fmt::format("{} = {} ", i, gamepad.buttons[i]).c_str());
                    }
                    for (int i = 0; i < gamepad.num_axes; i++) {
                        ImGui::Text("%s", fmt::format("{} = {} ", i, gamepad.axes[i]).c_str());
                    }
                }
            }
            ImGui::End();
            // ImGui::SFML::Render(window);
        }

        return event::Action::Pass;
    });

    for (size_t jid = 0; jid < ARR_LEN(gamepads); ++jid) {
        gamepads[jid].init(jid);

        // Make sure we set up any gamepads already connected
        if (sf::Joystick::isConnected(jid)) {
            event::sfml::post(event::sfml::Event{
                .type = event::sfml::Type::JoystickConnected,
                .joystickConnect = event::sfml::Event::JoystickConnectEvent{.joystickId = (unsigned int)jid}});
        }
    }
}

static void update() {
    for (uint jid = 0; jid < ARR_LEN(gamepads); ++jid) {
        auto &gamepad = gamepads[jid];

        if (gamepad.present) {
            auto &axes = gamepad.axes;
            auto &buttons = gamepad.buttons;

            for (int i = 0; i < gamepad.num_buttons; ++i) {
                bool new_state = sf::Joystick::isButtonPressed(jid, i);
                if (new_state && ((buttons[i] == PRESS) || (buttons[i] == REPEAT))) {
                    buttons[i] = REPEAT;
                } else {
                    buttons[i] = new_state ? PRESS : RELEASE;
                }
            }

            sf::Joystick::Axis axis[] = {
                sf::Joystick::Axis::X, sf::Joystick::Axis::Y, sf::Joystick::Axis::Z,    sf::Joystick::Axis::R,
                sf::Joystick::Axis::U, sf::Joystick::Axis::V, sf::Joystick::Axis::PovX, sf::Joystick::Axis::PovY,
            };

            for (uint i = 0; i < gamepad.num_axes; ++i) {
                axes[i] = sf::Joystick::getAxisPosition(jid, axis[i]);
            }

            if (buttons[GUIDE] && (buttons[START] == PRESS)) {
                do_debug ^= true;
                // fmt::print("\x1b[2J\n"); // clear screen
            }

            if (buttons[GUIDE] && (buttons[BUTTON_X] == PRESS)) {
                event::schd::post(event::schd::Event{.type = event::schd::Type::Do_Shutdown, .shutdown = {}});
            }
        }
    }
}

// bool check_quit()
// {
//     for (size_t jid = 0; jid < ARR_LEN(gamepads); ++jid) {
//         if (gamepads[jid].present) {
//             auto buttons = gamepads[jid].state.buttons;
//             if (buttons[GUIDE] && (buttons[BUTTON_X] == PRESS))
//                 return true;
//         }
//     }
//     return false;
// }
} // namespace gamepad
