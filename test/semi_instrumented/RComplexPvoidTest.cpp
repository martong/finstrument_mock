#include "FooFixture.hpp"

_Complex double RComplexPvoidCaller();
_Complex double RComplexPvoid();

_Complex double fake_RComplexPvoid(){
    return {2,5};
}

TEST_F(FooFixture, RComplexPvoid) {
    SUBSTITUTE(RComplexPvoid, fake_RComplexPvoid);
    auto res = RComplexPvoidCaller();

    _Complex double expected = {2, 5};
    EXPECT_EQ(res, expected);
}

