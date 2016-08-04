#pragma once

#include "hook.hpp"

template <typename T>
char *address(T t) {
    return ::fake::address(t);
}
