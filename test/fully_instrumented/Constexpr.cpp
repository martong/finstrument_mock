#include "FooFixture.hpp"

namespace {

constexpr int foo(int p) { return p*p; }
int fake_foo(int p) { return p*p*p; }

int bar(int p) {
    return foo(p);
}

} // unnamed

TEST_F(FooFixture, Constexpr) {
    static_assert(foo(2) == 4, "");
    SUBSTITUTE(&foo, &fake_foo);
    static_assert(foo(2) == 4, "");
    EXPECT_EQ(bar(2), 8);
}
