// Copyright (C) Tubbles github.com/Tubbles

#pragma once

#include "body.hpp"
#include "collision.hpp"
#include "event.hpp"
#include "log.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

struct Paddle {
    enum class Player {
        One,
        Two,
    };
    explicit Paddle(Player a_player) : player(a_player) {
        body = std::make_shared<Body>();
        body->shape = &shape;
        col::register_body(body);
        if (player == Player::One) {
            keymap = {sf::Keyboard::A, sf::Keyboard::Z, sf::Keyboard::Q};
            shape.setPosition({10, 240});
        } else {
            keymap = {sf::Keyboard::P, sf::Keyboard::SemiColon, sf::Keyboard::Slash};
            shape.setPosition({620, 240});
        }

        event::sfml::register_callback(event::sfml::Type::KeyPressed, 500, [&](event::sfml::Event event) {
            auto key = event.key.code;

            if (key == keymap[Up]) {
                up_vel = -1.0f;
            }
            if (key == keymap[Down]) {
                down_vel = 1.0f;
            }
            if (key == keymap[Speed]) {
                speed = fast_speed;
            }

            return event::Action::Pass;
        });

        event::sfml::register_callback(event::sfml::Type::KeyReleased, 500, [&](event::sfml::Event event) {
            auto key = event.key.code;

            if (key == keymap[Up]) {
                up_vel = 0.0f;
            }
            if (key == keymap[Down]) {
                down_vel = 0.0f;
            }
            if (key == keymap[Speed]) {
                speed = slow_speed;
            }

            return event::Action::Pass;
        });

        event::schd::register_callback(event::schd::Type::Update, 500, [&](event::schd::Event event) {
            (void)event;
            // sf::Time time = event.update.delta;
            body->velocity = {0.0f, (up_vel + down_vel) * speed};
            // shape.move(body.velocity.x, body.velocity.y);

            // // Check collision
            // for (auto col : col::get(body)) {
            //     col::move(body, col, col::Move::Me);
            //     spdlog::debug("Collision detected!");
            // }
            // auto pos = shape.getPosition();
            // auto size = shape.getSize();
            // static const sf::Vector2f upper_lim{0.0f, 0.0f};
            // static const sf::Vector2f lower_lim{640.0f, 480.0f};
            // if (pos.y < upper_lim.y) {
            //     shape.setPosition(pos.x, upper_lim.y);
            // }
            // if (pos.y + size.y > lower_lim.y) {
            //     shape.setPosition(pos.x, lower_lim.y - size.y);
            // }
            return event::Action::Pass;
        });

        event::schd::register_callback(event::schd::Type::Draw, 500, [&](event::schd::Event event) {
            event.draw.window->draw(*body->shape);
            return event::Action::Pass;
        });
    }

private:
    enum Keymap {
        Up = 0,
        Down = 1,
        Speed = 2,
    };
    std::array<sf::Keyboard::Key, 3> keymap{};
    Player player;
    sf::RectangleShape shape{{10, 40}};
    std::shared_ptr<Body> body{};
    float up_vel{}, down_vel{};
    static constexpr float slow_speed = 400.0f;
    static constexpr float fast_speed = 800.0f;
    float speed = slow_speed;
};
