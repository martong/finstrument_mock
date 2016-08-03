#include "FooFixture.hpp"
#include "Aggr.hpp"

Aggr RAggrCaller(Aggr x);
Aggr RAggr(Aggr);
Aggr RAggr_2(Aggr);

Aggr fake_RAggr(Aggr pa){
    ::ftest::called.insert(address(&fake_RAggr));
    EXPECT_EQ(13, pa.a);
    EXPECT_EQ(17, pa.b);
    return {pa.a * 2, pa.b * 2};
}

TEST_F(FooFixture, RAggrLvalue) {
    ::fake::subs.insert(
        {address(RAggr), address(fake_RAggr)});
    Aggr result = RAggrCaller(Aggr{13, 17});

    EXPECT_EQ(result.a, 26);
    EXPECT_EQ(result.b, 34);

    EXPECT_EQ(::ftest::called.count(address(RAggr)), 0);
    EXPECT_EQ(::ftest::called.count(address(fake_RAggr)), 1);
    EXPECT_EQ(::ftest::called.count(address(RAggr_2)), 1);
}

TEST_F(FooFixture, RAggrRvalue) {
    ::fake::subs.insert(
        {address(RAggr), address(fake_RAggr)});

    EXPECT_EQ(RAggrCaller(Aggr{13, 17}).a, 26);
    EXPECT_EQ(RAggrCaller(Aggr{13, 17}).b, 34);

    EXPECT_EQ(::ftest::called.count(address(RAggr)), 0);
    EXPECT_EQ(::ftest::called.count(address(fake_RAggr)), 1);
    EXPECT_EQ(::ftest::called.count(address(RAggr_2)), 1);
}
