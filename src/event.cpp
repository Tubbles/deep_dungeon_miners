// Copyright (C) Tubbles github.com/Tubbles

#include "event.hpp"

namespace event {
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
template <> sfml::CallbackMap sfml::callbacks{};
// NOLINTNEXTLINE(cppcoreguidelines-avoid-non-const-global-variables)
template <> schd::CallbackMap schd::callbacks{};
} // namespace event
