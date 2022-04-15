#pragma once

#include <functional>

namespace Cookie {

template <typename T> using Func = std::function<T>;

}