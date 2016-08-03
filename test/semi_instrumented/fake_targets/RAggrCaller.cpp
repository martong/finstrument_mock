#include "Aggr.hpp"
Aggr RAggr(Aggr);
Aggr RAggr_2(Aggr);

Aggr RAggrCaller(Aggr x) {
  RAggr_2(x);
  return RAggr(x);
}
