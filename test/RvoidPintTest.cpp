#include "FooFixture.hpp"

void RvoidPintCaller();
void RvoidPint(int);
void RvoidPint_2(int);

#define FAKE_AUX_FUN(fake_fun)                                            \
    void fake_fun(int a);                                                 \
    void fake_fun##_aux(int count, ...) {                                 \
        ::ftest::called.insert(reinterpret_cast<char *>(fake_fun##_aux)); \
                                                                          \
        EXPECT_EQ(count, 1);                                              \
                                                                          \
        va_list args;                                                     \
        va_start(args, count);                                            \
        fake_fun(va_arg(args, int));                                      \
        va_end(args);                                                     \
    }

FAKE_AUX_FUN(fake_RvoidPint)

// User provides
void fake_RvoidPint(int a) {
    ::ftest::called.insert(reinterpret_cast<char *>(fake_RvoidPint));
    EXPECT_EQ(a, 13);
}

/// signature: void(int)
TEST_F(FooFixture, RvoidPint) {
    ::fake::subs.insert({address(RvoidPint), address(fake_RvoidPint_aux)});
    RvoidPintCaller();

    EXPECT_EQ(::ftest::called.count(address(RvoidPint)), 0);
    EXPECT_EQ(::ftest::called.count(address(fake_RvoidPint_aux)), 1);
    EXPECT_EQ(::ftest::called.count(address(fake_RvoidPint)), 1);
    EXPECT_EQ(::ftest::called.count(address(RvoidPint_2)), 1);
}


void RvoidPintCaller_callsTwice();
void RvoidPint_3(int);

FAKE_AUX_FUN(fake_RvoidPint_3)

// User provides
void fake_RvoidPint_3(int a) {
    ::ftest::called.insert(reinterpret_cast<char *>(fake_RvoidPint_3));
    static int called = 0;
    if (called == 0) {
        EXPECT_EQ(a, 13);
    }
    if (called == 1) {
        EXPECT_EQ(a, 42);
    }
    ++called;
}

TEST_F(FooFixture, RvoidPint_callsTwice) {
    ::fake::subs.insert({address(RvoidPint_3), address(fake_RvoidPint_3_aux)});
    RvoidPintCaller_callsTwice();

    EXPECT_EQ(::ftest::called.count(address(RvoidPint_3)), 0);
    EXPECT_EQ(::ftest::called.count(address(fake_RvoidPint_3)), 1);
}


void RvoidPintCaller_lvalue(int);
void RvoidPint_4(int);

FAKE_AUX_FUN(fake_RvoidPint_4)

// User provides
void fake_RvoidPint_4(int a) {
    ::ftest::called.insert(reinterpret_cast<char *>(fake_RvoidPint_4));
    EXPECT_EQ(a, 16);
}

TEST_F(FooFixture, RvoidPint_lvalue) {
    ::fake::subs.insert({address(RvoidPint_4), address(fake_RvoidPint_4_aux)});
    RvoidPintCaller_lvalue(4);

    EXPECT_EQ(::ftest::called.count(address(RvoidPint_4)), 0);
    EXPECT_EQ(::ftest::called.count(address(fake_RvoidPint_4)), 1);
}


void PhiProblem(int);
void RvoidPint_5(int);

FAKE_AUX_FUN(fake_RvoidPint_5)

// User provides
void fake_RvoidPint_5(int a) {
    ::ftest::called.insert(reinterpret_cast<char *>(fake_RvoidPint_5));
    EXPECT_EQ(a, 25);
}

TEST_F(FooFixture, PhiProblem) {
    ::fake::subs.insert({address(RvoidPint_5), address(fake_RvoidPint_5_aux)});
    PhiProblem(5);

    EXPECT_EQ(::ftest::called.count(address(RvoidPint_5)), 0);
    EXPECT_EQ(::ftest::called.count(address(fake_RvoidPint_5)), 1);
}
