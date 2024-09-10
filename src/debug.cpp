// Copyright (C) Tubbles github.com/Tubbles

#include "debug.hpp"
#include "event.hpp"
#include "imgui-SFML.h"
#include "imgui.h"
#include "log.hpp"
#include "util.hpp"

namespace debug {
void setup(fs::path &my_dir, sf::RenderWindow &window) {
    static const std::string imgui_ini_filename{my_dir.string() + std::string{"/imgui.ini"}};
    static const std::string imgui_log_filename{my_dir.string() + std::string{"/imgui_log.txt"}};

    static bool show_imgui = false;

    if (!ImGui::SFML::Init(window)) {
        spdlog::error("ImGui initialization failed");
    }

    ImGui::GetIO().IniFilename = imgui_ini_filename.c_str();
    ImGui::GetIO().LogFilename = imgui_log_filename.c_str();

    event::sfml::register_callback(event::sfml::Type::KeyPressed, PRIO_NORMAL, [&](event::sfml::Event event) {
        if (event.key.code == sf::Keyboard::F1) {
            show_imgui = !show_imgui;
        }
        return event::Action::Pass;
    });

    event::schd::register_callback(event::schd::Type::Update, PRIO_NORMAL, [&](event::schd::Event event) {
        auto time = event.update.delta;
        ImGui::SFML::Update(window, time);
        return event::Action::Pass;
    });

    event::schd::register_callback(event::schd::Type::Draw, PRIO_NORMAL, [&](event::schd::Event event) {
        UNUSED(event);
        if (show_imgui) {
            ImGui::Begin("FPS");
            ImGui::Text("%s", fmt::format("{:.1f}", ImGui::GetIO().Framerate).c_str());
            ImGui::End();
        }

        return event::Action::Pass;
    });

    event::schd::register_callback(event::schd::Type::Shutdown, PRIO_NORMAL, [&](event::schd::Event) {
        ImGui::SFML::Shutdown();
        return event::Action::Pass;
    });

    spdlog::info("ImGui started, press F1 to toggle overlay");
}
} // namespace debug
