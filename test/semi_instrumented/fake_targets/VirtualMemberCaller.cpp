#include "VirtX.hpp"

void VirtualMemberCaller(VirtX& x, int ip) {
  x.foo(ip);
  //x.foo_2(ip * 2);
}
