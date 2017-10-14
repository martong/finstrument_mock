#include "FooFixture.hpp"

namespace {
    int bar() { return 13; }
    int foo() {
        auto result = bar();
        return result;
    }

    struct BarE {};
    int fake_bar() { throw BarE{}; return 15; }
}

TEST_F(FooFixture, Exception) {
    EXPECT_NO_THROW(foo());

    SUBSTITUTE(bar, fake_bar);
    EXPECT_THROW(foo(), BarE);
}
