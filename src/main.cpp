// Copyright (C) Tubbles github.com/Tubbles

#include "body.hpp"
#include "collision.hpp"
#include "event.hpp"
#include "gamepad.hpp"
#include "imgui-SFML.h"
#include "imgui.h"
#include "log.hpp"
#include "paddle.hpp"
#include "timer.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <fmt/core.h>

#ifndef NDEBUG
#include "debug.hpp"
#endif

auto main(int argc, char *argv[]) -> int {
    std::vector<std::string> args(argv, argv + argc);
    fs::path prog{args[0]};
    fs::path my_dir{fs::canonical(prog.parent_path()).string()};

    lg::setup(my_dir);
    spdlog::info("Application started");

    for (auto &arg : args) {
        spdlog::info("Arg #{}#", arg);
    }

    sf::RenderWindow window(sf::VideoMode(640, 480), "SFML Tutorial");
    window.setVerticalSyncEnabled(true); // Cap at 60 Hz

#ifndef NDEBUG
    debug::setup(my_dir, window);
#endif

    gamepad::setup();

    col::init();

    sf::RectangleShape roof{{640, 10}};
    sf::RectangleShape floor{{640, 10}};
    roof.setPosition(0, -10);
    floor.setPosition(0, 480);
    col::register_body({.shape = &roof, .is_static = true, .plasticity = std::numeric_limits<float>::infinity()});
    col::register_body({.shape = &floor, .is_static = true, .plasticity = std::numeric_limits<float>::infinity()});
    Paddle paddle1{Paddle::Player::One};
    Paddle paddle2{Paddle::Player::Two};

    sf::Clock deltaClock;

    // Set up global timers
    event::schd::register_callback(event::schd::Type::Update, 500, [&window](event::schd::Event event) {
        Timer::update_global(event.update.delta);
        return event::Action::Pass;
    });

    event::sfml::register_callback(event::sfml::Type::Closed, 500, [&window](event::sfml::Event event) {
        (void)event;
        window.close();
        return event::Action::Block;
    });

    event::sfml::register_callback(event::sfml::Type::KeyPressed, 500, [&window](event::sfml::Event event) {
        if (event.key.code == sf::Keyboard::Escape) {
            window.close();
            return event::Action::Pass;
        }
        return event::Action::Pass;
    });

    event::schd::register_callback(event::schd::Type::Do_Shutdown, 500, [&window](event::schd::Event event) {
        UNUSED(event);
        window.close();
        return event::Action::Pass;
    });

    // Main game loop
    while (window.isOpen()) {
        sf::Event event{};
        while (window.pollEvent(event)) {
#ifndef NDEBUG
            ImGui::SFML::ProcessEvent(window, event);
#endif

            event::sfml::post(event);
        }

        // Update
        event::schd::post(
            event::schd::Event{.type = event::schd::Type::Update, .update = {.delta = deltaClock.restart()}});

        // Draw
        window.clear();
        // window.draw(shape);
        // ImGui::NewFrame();
        event::schd::post(event::schd::Event{.type = event::schd::Type::Draw, .draw = {.window = &window}});
        // ImGui::EndFrame();
        ImGui::SFML::Render(window);
        window.display();
    }

    // Shutdown
    event::schd::post(event::schd::Event{.type = event::schd::Type::Shutdown, .shutdown{}});

    return 0;
}
