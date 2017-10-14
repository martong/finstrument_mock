#include "FooFixture.hpp"

using Fun = void();
using FunPtr = Fun *;
void FunPtrCaller(FunPtr fp);

namespace {
void foo() { ::ftest::called.insert(address(&foo)); };
void baz() { FunPtrCaller(foo); }
void fake_foo() { ::ftest::called.insert(address(&fake_foo)); }
}

TEST_F(FooFixture, FunPointer1) {
    baz();
    EXPECT_EQ(::ftest::called.count(address(foo)), 1);
}

TEST_F(FooFixture, FunPointer2) {
    SUBSTITUTE(foo, fake_foo);
    baz();
    EXPECT_EQ(::ftest::called.count(address(foo)), 0);
    EXPECT_EQ(::ftest::called.count(address(fake_foo)), 1);
}
