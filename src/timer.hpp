// Copyright (C) Tubbles github.com/Tubbles

#pragma once

#include "base.hpp"
#include "log.hpp"
#include <SFML/System/Time.hpp>
#include <functional>
#include <vector>

struct Timer {
    using Callback = std::function<void()>;

    sf::Time time{};
    sf::Time period{};
    sf::Time total_accumulated_time{};
    Callback callback{};
    bool is_one_shot = false;
    size_t num_fires = {};

    Timer(sf::Time period_, Callback callback_, bool is_one_shot_ = false)
        : period(period_), callback(callback_), is_one_shot(is_one_shot_) {}

    void update(sf::Time time_) {
        time = time + time_;
        total_accumulated_time += time_;
        if (time > period) {
            time -= period;
            callback();
            num_fires += 1;
        }
    }

    // Global timers
    static std::vector<Timer> timers;

    static void add_global(sf::Time period, Callback callback) { timers.push_back(Timer{period, callback}); }

    static void add_one_shot_global(sf::Time period, Callback callback) {
        timers.push_back(Timer{period, callback, true});
    }

    static void update_global(sf::Time time) {
        for (auto timer = timers.begin(); timer != timers.end();) {
            timer->update(time);
            if ((timer->num_fires > 0) && (timer->is_one_shot)) {
                timer = timers.erase(timer);
            } else {
                ++timer;
            }
        }
    }
};
