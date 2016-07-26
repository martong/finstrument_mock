#include "FooFixture.hpp"

// The function that the user wants to test with replacing call expressions
// inside.
void RvoidPvoidCaller();
// The used functions inside that the user might replace
void RvoidPvoid();
void RvoidPvoid_2();

void fake_RvoidPvoid(){
    ::ftest::called.insert(address(&fake_RvoidPvoid));
}

/// signature: void()
TEST_F(FooFixture, RvoidPvoid) {
    ::fake::subs.insert(
        {address(RvoidPvoid), address(fake_RvoidPvoid)});
    RvoidPvoidCaller();

    EXPECT_EQ(::ftest::called.count(address(RvoidPvoid)), 0);
    EXPECT_EQ(::ftest::called.count(address(fake_RvoidPvoid)), 1);
    EXPECT_EQ(::ftest::called.count(address(RvoidPvoid_2)), 1);
}
