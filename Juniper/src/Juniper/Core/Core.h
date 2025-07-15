#pragma once

#include <functional>

#define JP_BIND_HANDLER(x) std::bind(&x, this, std::placeholders::_1)