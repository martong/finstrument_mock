#include "FooFixture.hpp"

namespace {

    int bar() { return 13; }

    int foo() {
        auto result = bar();
        return result;
    }

    // In order to call the original function,
    // we must avoid the instrumentation of the fake function.
    // Otherwise an infinite loop would be started from the call expression.
    // GNU flavour: __attribute__((no_sanitize("mock")))
    [[clang::no_sanitize("mock")]]
    int fake_bar() { return bar() * 2; }

}

TEST_F(FooFixture, Recursion) {
    SUBSTITUTE(&bar, &fake_bar);
    EXPECT_EQ(foo(), 26);
}
