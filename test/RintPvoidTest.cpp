#include "FooFixture.hpp"

// The function that the user wants to test with replacing call expressions
// inside.
int RintPvoidCaller();
// The used functions inside that the user might replace
int RintPvoid();
int RintPvoid_2();

int fake_RintPvoid() {
    ::ftest::called.insert(address(&fake_RintPvoid));
    return 42;
}

TEST_F(FooFixture, RintPvoidReplaced) {
    ::fake::subs.insert(
        {address(RintPvoid), address(fake_RintPvoid)});
    auto res = RintPvoidCaller();

    EXPECT_EQ(::ftest::called.count(address(RintPvoid)), 0);
    EXPECT_EQ(::ftest::called.count(address(fake_RintPvoid)), 1);
    EXPECT_EQ(::ftest::called.count(address(RintPvoid_2)), 1);
    EXPECT_EQ(res, 42);
}

TEST_F(FooFixture, RintPvoidOriginal) {
    auto res = RintPvoidCaller();
    EXPECT_EQ(::ftest::called.count(address(RintPvoid)), 1);
    EXPECT_EQ(::ftest::called.count(address(fake_RintPvoid)), 0);
    EXPECT_EQ(::ftest::called.count(address(RintPvoid_2)), 1);
    EXPECT_EQ(res, 13); // 13 comes from the definition of RintPvoid()
}
