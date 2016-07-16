#include <cstdio>
#include "foo.hpp"

#include "called.hpp"

void fooRvoidPvoid() {
    ::ftest::called.insert(reinterpret_cast<char*>(fooRvoidPvoid));
}

void fooRvoidPvoid_2() {
    ::ftest::called.insert(reinterpret_cast<char*>(fooRvoidPvoid_2));
}

void fooRvoidPint(int a) {
    ::ftest::called.insert(reinterpret_cast<char*>(fooRvoidPint));
}

void fooRvoidPint_2(int a) {
    ::ftest::called.insert(reinterpret_cast<char*>(fooRvoidPint_2));
}
