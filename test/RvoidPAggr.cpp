#include "called.hpp"
#include "Aggr.hpp"

void RvoidPAggr(Aggr) {
    ::ftest::called.insert(reinterpret_cast<char*>(RvoidPAggr));
}

void RvoidPAggr_2(Aggr) {
    ::ftest::called.insert(reinterpret_cast<char*>(RvoidPAggr_2));
}
