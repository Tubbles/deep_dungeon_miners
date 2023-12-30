// Copyright (C) Tubbles github.com/Tubbles

#pragma once

#include "base.hpp"

#define ARR_LEN(a) sizeof(a) / sizeof(a[0])
#define ARR_ZERO(a) std::memset(a, 0, sizeof(a))
#define UNUSED(x) (void)(x)
