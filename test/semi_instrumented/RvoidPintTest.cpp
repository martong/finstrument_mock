#include "FooFixture.hpp"

void RvoidPintCaller();
void RvoidPint(int);
void RvoidPint_2(int);

void fake_RvoidPint(int a) {
    ::ftest::called.insert(address(&fake_RvoidPint));
    EXPECT_EQ(a, 13);
}

/// signature: void(int)
TEST_F(FooFixture, RvoidPint) {
    SUBSTITUTE(RvoidPint, fake_RvoidPint);
    RvoidPintCaller();

    EXPECT_EQ(::ftest::called.count(address(RvoidPint)), 0);
    EXPECT_EQ(::ftest::called.count(address(fake_RvoidPint)), 1);
    EXPECT_EQ(::ftest::called.count(address(RvoidPint_2)), 1);
}


void RvoidPintCaller_callsTwice();
void RvoidPint_3(int);

// User provides
void fake_RvoidPint_3(int a) {
    ::ftest::called.insert(address(&fake_RvoidPint_3));
    static int called = 0;
    if (called == 0) {
        EXPECT_EQ(a, 13);
    }
    else {
        EXPECT_EQ(called, 1);
        EXPECT_EQ(a, 42);
    }
    ++called;
}

TEST_F(FooFixture, RvoidPint_callsTwice) {
    SUBSTITUTE(RvoidPint_3, fake_RvoidPint_3);
    RvoidPintCaller_callsTwice();

    EXPECT_EQ(::ftest::called.count(address(RvoidPint_3)), 0);
    EXPECT_EQ(::ftest::called.count(address(fake_RvoidPint_3)), 1);
}


void RvoidPintCaller_lvalue(int);
void RvoidPint_4(int);

void fake_RvoidPint_4(int a) {
    ::ftest::called.insert(address(&fake_RvoidPint_4));
    EXPECT_EQ(a, 16);
}

TEST_F(FooFixture, RvoidPint_lvalue) {
    SUBSTITUTE(RvoidPint_4, fake_RvoidPint_4);
    RvoidPintCaller_lvalue(4);

    EXPECT_EQ(::ftest::called.count(address(RvoidPint_4)), 0);
    EXPECT_EQ(::ftest::called.count(address(fake_RvoidPint_4)), 1);
}


void PhiProblem(int);
void RvoidPint_5(int);

void fake_RvoidPint_5(int a) {
    ::ftest::called.insert(address(&fake_RvoidPint_5));
    EXPECT_EQ(a, 25);
}

TEST_F(FooFixture, PhiProblem) {
    SUBSTITUTE(RvoidPint_5, fake_RvoidPint_5);
    PhiProblem(5);

    EXPECT_EQ(::ftest::called.count(address(RvoidPint_5)), 0);
    EXPECT_EQ(::ftest::called.count(address(fake_RvoidPint_5)), 1);
}
