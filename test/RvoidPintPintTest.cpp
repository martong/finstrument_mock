#include "FooFixture.hpp"

void RvoidPintPintCaller();
void RvoidPintPint(int, int);
void RvoidPintPint_2(int, int);

void fake_RvoidPintPint(int a0, int a1) {
    ::ftest::called.insert(reinterpret_cast<char *>(fake_RvoidPintPint));
    EXPECT_EQ(a0, 13);
    EXPECT_EQ(a1, 13);
}

/// signature: void(int)
TEST_F(FooFixture, RvoidPintPint) {
    ::fake::subs.insert({address(RvoidPintPint), address(fake_RvoidPintPint)});
    RvoidPintPintCaller();

    EXPECT_EQ(::ftest::called.count(address(RvoidPintPint)), 0);
    EXPECT_EQ(::ftest::called.count(address(fake_RvoidPintPint)), 1);
    EXPECT_EQ(::ftest::called.count(address(RvoidPintPint_2)), 1);
}
