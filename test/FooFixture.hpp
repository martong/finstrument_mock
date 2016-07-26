#pragma once

#include <gtest/gtest.h>

#include "hook.hpp"
#include "called.hpp"

struct FooFixture : ::testing::Test {
    FooFixture() {
        ::ftest::called.clear();
        ::fake::subs.clear();
    }
};
