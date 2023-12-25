// Copyright (C) Tubbles github.com/Tubbles

#pragma once

#include "log.hpp"
#include "schd_event.hpp"
#include <SFML/Window/Event.hpp>
#include <functional>
#include <map>
#include <typeinfo>

namespace event {
enum class Action {
    Pass,  // Pass on the event to the next handler
    Block, // Finish parsing this event
};
template <typename TypeT, typename EventT> struct Manager {
    using Type = TypeT;
    using Event = EventT;
    // Callback typedef, return true if no more event processing shall occur for this event
    using Callback = std::function<Action(Event)>;
    struct PrioCallback {
        Callback callback;
        uint32_t prio;
    };

    using CallbackMap = std::map<Type, std::vector<PrioCallback>>;
    // NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
    static CallbackMap callbacks;

    static void register_callback(Type type, uint32_t priority, const Callback &callback) {
        spdlog::debug("Attempting to register {} callback for event type {} with priority {}", typeid(Event).name(),
                      (int)type, priority);
        auto &vec = callbacks[type];

        for (auto it = vec.begin(); it != vec.end(); ++it) {
            if (priority < (*it).prio) {
                vec.insert(it, {callback, priority});
                spdlog::debug("Registered {} callback for event type {} with priority {} (insert)",
                              typeid(Event).name(), (int)type, priority);
                return;
            }
        }

        vec.push_back({callback, priority});
        spdlog::debug("Registered {} callback for event type {} with priority {} (push)", typeid(Event).name(),
                      (int)type, priority);
    }

    static void post(Event event) {
        auto &vec = callbacks[event.type];
        for (auto &elem : vec) {
            if (elem.callback(event) == Action::Block) {
                break;
            }
        }
    }
};

using sfml = Manager<sf::Event::EventType, sf::Event>;
using schd = Manager<schd::Event::EventType, schd::Event>;
} // namespace event
