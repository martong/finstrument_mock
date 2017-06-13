#pragma once

#include "hook.hpp"

template <typename T>
const char *address(T t) {
    return ::fake::address(t);
}
