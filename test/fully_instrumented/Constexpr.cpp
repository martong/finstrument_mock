#include "FooFixture.hpp"

namespace {

constexpr int foo(int p) { return p*p; }
int fake_foo(int p) { return p*p*p; }

constexpr int bar(int p) {
    return foo(p);
}

} // unnamed

TEST_F(FooFixture, Constexpr) {
    SUBSTITUTE(&foo, &fake_foo);

    static_assert(foo(2) == 4, "");
    EXPECT_EQ(foo(2), 8);
    int p = 2;
    EXPECT_EQ(foo(p), 8);

    static_assert(bar(2) == 4, "");
    EXPECT_EQ(bar(2), 8);
    EXPECT_EQ(bar(p), 8);
}
