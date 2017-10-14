#include "FooFixture.hpp"
#include "X.hpp"

void MemberCaller(X&, int);

X global_x;
void fake_foo(X* self, int p){
    ::ftest::called.insert(address(&fake_foo));
    EXPECT_EQ(self, &global_x);
    EXPECT_EQ(p, 13);
}
TEST_F(FooFixture, MemberCall) {
    SUBSTITUTE(X::foo, fake_foo);

    MemberCaller(global_x, 13);

    EXPECT_EQ(::ftest::called.count(address(&X::foo)), 0);
    EXPECT_EQ(::ftest::called.count(address(&fake_foo)), 1);
    EXPECT_EQ(::ftest::called.count(address(&X::foo_2)), 1);
}

X global_y;
void fake_foo_2(X* self, int p){
    ::ftest::called.insert(address(&fake_foo_2));
    static int called = 0;
    if (called == 0) {
        EXPECT_EQ(self, &global_x);
        EXPECT_EQ(p, 13);
    } else {
        EXPECT_EQ(self, &global_y);
        EXPECT_EQ(p, 17);
    }
    ++called;
}
TEST_F(FooFixture, MemberCall_two_objects) {
    SUBSTITUTE(X::foo, fake_foo_2);

    MemberCaller(global_x, 13);
    MemberCaller(global_y, 17);

    EXPECT_EQ(::ftest::called.count(address(&X::foo)), 0);
    EXPECT_EQ(::ftest::called.count(address(&fake_foo_2)), 1);
}
