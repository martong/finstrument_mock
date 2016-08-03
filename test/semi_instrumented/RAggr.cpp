#include "called.hpp"
#include "Aggr.hpp"

Aggr RAggr(Aggr) {
    ::ftest::called.insert(reinterpret_cast<char*>(RAggr));
    return {0,0};
}

Aggr RAggr_2(Aggr) {
    ::ftest::called.insert(reinterpret_cast<char*>(RAggr_2));
    return {0, 0};
}
