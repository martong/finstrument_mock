#include "X.hpp"

void MemberCaller(X& x, int ip) {
  x.foo(ip);
  x.foo_2(ip * 2);
}
