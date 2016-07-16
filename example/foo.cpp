#include <cstdio>
#include "foo.hpp"

#include "called.hpp"

void fooRvoidPint(int a) {
    test::called.insert(reinterpret_cast<char*>(fooRvoidPint));
}

void fooRvoidPint_2(int a) {
    test::called.insert(reinterpret_cast<char*>(fooRvoidPint_2));
}
