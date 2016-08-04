#pragma once

#include <gtest/gtest.h>

#include "hook.hpp"
#include "address.hpp"
#include "called.hpp"

struct FooFixture : ::testing::Test {
    FooFixture() {
        ::ftest::called.clear();
        ::fake::clear();
    }
};
