#include "foo.hpp"

int square(int a) { return a*a; }

void bar() {
  // TODO separate TUs for different tests!!!
  fooRvoidPvoid();
  fooRvoidPvoid_2();
  fooRvoidPint(13);
  fooRvoidPint_2(15);
  //fooRvoidPint(square(3));
}
