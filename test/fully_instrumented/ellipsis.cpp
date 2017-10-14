#include <gtest/gtest.h>

#include "FooFixture.hpp"

int add_nums(int count, ...)
{
    int result = 0;
    va_list args;
    va_start(args, count);
    for (int i = 0; i < count; ++i) {
        result += va_arg(args, int);
    }
    va_end(args);
    return result;
}

int mul_nums(int count, ...)
{
    int result = 1;
    va_list args;
    va_start(args, count);
    for (int i = 0; i < count; ++i) {
        result *= va_arg(args, int);
    }
    va_end(args);
    return result;
}

TEST_F(FooFixture, Ellipsis) {
    EXPECT_EQ(add_nums(4, 1, 2, 3, 4), 10);
    EXPECT_EQ(mul_nums(4, 1, 2, 3, 4), 24);
    SUBSTITUTE(add_nums, mul_nums);
    EXPECT_EQ(add_nums(4, 1, 2, 3, 4), 24);
}
