#pragma once

#include <gtest/gtest.h>

// The user should not need to handle varargs
// Later, a macro will generate the functions that use varargs
#include <cstdarg>

#include "hook.hpp"

#include "called.hpp"

template <typename T>
char *address(T t) {
    return reinterpret_cast<char *>(t);
}

struct FooFixture : ::testing::Test {
    FooFixture() {
    ::ftest::called.clear();
    ::fake::subs.clear();
    }
};
