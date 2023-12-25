// Copyright (C) Tubbles github.com/Tubbles

#pragma once

#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <memory>

struct Body {
    sf::Shape *shape{};
    sf::Vector2f velocity{};
    bool is_static{false}; // moves during collision solving
    float mass{};
    float plasticity{}; // 0 = fully elastic collisions (bounce), inf = fully plastic collisions (absorb)
};
