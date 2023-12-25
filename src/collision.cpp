// Copyright (C) Tubbles github.com/Tubbles

#include "collision.hpp"
#include "event.hpp"
#include "log.hpp"
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <memory>

namespace col {

// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
static std::vector<std::shared_ptr<Body>> bodies{};

// NOLINTNEXTLINE(readability-identifier-length)
static auto rect_col(const sf::RectangleShape &r1, const sf::RectangleShape &r2) -> bool {
    auto pos = r1.getPosition();
    auto size = r1.getSize();
    auto opos = r2.getPosition();
    auto osize = r2.getSize();

    // clang-format off
    return ((pos.x < (opos.x + osize.x)) && ((pos.x + size.x) > opos.x) &&
            (pos.y < (opos.y + osize.y)) && ((pos.y + size.y) > opos.y));
    // clang-format on
}

// // NOLINTNEXTLINE(readability-identifier-length)
// static auto circ_col(const sf::CircleShape &c1, const sf::CircleShape &c2) -> bool {
//     (void)c1;
//     (void)c2;
//     // TODO
//     return false;
// }

// // NOLINTNEXTLINE(readability-identifier-length)
// static auto rect_circ_col(const sf::RectangleShape &r, const sf::CircleShape &c) -> bool {
//     (void)r;
//     (void)c;
//     // TODO
//     return false;
// }

static auto check_cols(const sf::RectangleShape &shape) -> std::vector<Collision> {
    std::vector<Collision> vec{};

    for (auto body : bodies) {
        if (&shape == body->shape) {
            continue;
        }
        if (auto *oshape = dynamic_cast<sf::RectangleShape *>(body->shape); oshape) {
            if (rect_col(shape, *oshape)) {
                vec.push_back({.other = *body});
            }
        } else if (auto *oshape = dynamic_cast<sf::CircleShape *>(body->shape); oshape) {
            (void)oshape;
            // TODO
        }
    }

    return vec;
}

void init() {
    event::schd::register_callback(event::schd::Type::Update, 900, [&](event::schd::Event event) {
        float seconds = event.update.delta.asSeconds();

        // Move the bodies
        // spdlog::debug("num bodies {}", bodies.size());
        for (auto body : bodies) {
            body->shape->move(body->velocity.x * seconds, body->velocity.y * seconds);
        }

        // Check for collisions
        for (auto body : bodies) {
            for (auto col : get(*body)) {
                solve(*body, col, seconds);
                // spdlog::debug("Collision detected!");
            }
        }

        return event::Action::Pass;
    });
}

void register_body(std::shared_ptr<Body> body) { bodies.push_back(body); }
void register_body(Body &&body) { bodies.push_back(std::make_shared<Body>(body)); }

auto get(const Body &body) -> std::vector<Collision> {
    std::vector<Collision> vec{};
    (void)body;
    if (auto *shape = dynamic_cast<sf::RectangleShape *>(body.shape); shape) {
        vec = check_cols(*shape);
    } else if (auto *shape = dynamic_cast<sf::CircleShape *>(body.shape); shape) {
        (void)shape;
        // TODO
    }

    return vec;
}

// static bool col_overlap(const sf::RectangleShape &r1, const sf::RectangleShape &r2) {
//     auto pos = r1.getPosition();
//     auto size = r1.getSize();
//     auto opos = r2.getPosition();
//     auto osize = r2.getSize();

//     return ((pos.x < (opos.x + osize.x)) && ((pos.x + size.x) > opos.x) && (pos.y < (opos.y + osize.y)) &&
//             ((pos.y + size.y) > opos.y));
// }

void solve(const Body &body, const Collision &col, float seconds) {
    (void)seconds;
    if (body.is_static && col.other.is_static) {
        // Aint movin' no statics around
        return;
    } else if (col.other.is_static) {
        // Move me

    } else if (body.is_static) {
        // Move other
    } else {
        // Move both
    }
}
} // namespace col
