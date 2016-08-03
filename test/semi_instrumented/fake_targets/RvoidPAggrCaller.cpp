#include "Aggr.hpp"
void RvoidPAggr(Aggr);
void RvoidPAggr_2(Aggr);

void RvoidPAggrCaller(Aggr x) {
  RvoidPAggr(x);
  RvoidPAggr_2(x);
}
