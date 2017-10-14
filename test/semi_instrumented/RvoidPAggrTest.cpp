#include "FooFixture.hpp"
#include "Aggr.hpp"

void RvoidPAggrCaller(Aggr x);
void RvoidPAggr(Aggr);
void RvoidPAggr_2(Aggr);

Aggr global_a{13, 26};

void fake_RvoidPAggr(Aggr pa){
    ::ftest::called.insert(address(&fake_RvoidPAggr));
    EXPECT_EQ(global_a.a, pa.a);
    EXPECT_EQ(global_a.b, pa.b);
}

TEST_F(FooFixture, RvoidPAggr) {
    SUBSTITUTE(RvoidPAggr, fake_RvoidPAggr);
    RvoidPAggrCaller(global_a);

    EXPECT_EQ(::ftest::called.count(address(RvoidPAggr)), 0);
    EXPECT_EQ(::ftest::called.count(address(fake_RvoidPAggr)), 1);
    EXPECT_EQ(::ftest::called.count(address(RvoidPAggr_2)), 1);
}
