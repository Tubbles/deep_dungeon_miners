// Copyright (C) Tubbles github.com/Tubbles

#pragma once

#include "body.hpp"
#include <SFML/Graphics/Shape.hpp>
#include <variant>
#include <vector>

namespace col {
struct Collision {
    Body other{};
};

void init();
void register_body(std::shared_ptr<Body> body);
void register_body(Body &&);
auto get(const Body &) -> std::vector<Collision>;
void solve(const Body &, const Collision &, float seconds);
} // namespace col
