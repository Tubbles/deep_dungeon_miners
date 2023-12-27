// Copyright (C) Tubbles github.com/Tubbles

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Time.hpp>
#include <fmt/format.h>
#include <memory>

namespace schd {
struct Event {
    struct UpdateEvent {
        sf::Time delta{};
    };

    struct DrawEvent {
        sf::RenderWindow *window{};
    };

    struct ShutdownEvent {};

    enum EventType {
        // Output events
        Update,   // payload is UpdateEvent
        Draw,     // payload is DrawEvent
        Shutdown, // payload is ShutdownEvent

        // Input events
        Do_Shutdown, // payload is ShutdownEvent
    };

    EventType type{};

    union {
        UpdateEvent update;
        DrawEvent draw;
        ShutdownEvent shutdown;
    };
    // static auto format_as(const Event::EventType &type) { fmt::underlying(type); }
};

} // namespace schd
