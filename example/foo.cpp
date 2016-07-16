#include <cstdio>
#include "foo.hpp"

#include "called.hpp"

void fooRvoidPvoid() {
    test::called.insert(reinterpret_cast<char*>(fooRvoidPvoid));
}

void fooRvoidPvoid_2() {
    test::called.insert(reinterpret_cast<char*>(fooRvoidPvoid_2));
}

void fooRvoidPint(int a) {
    test::called.insert(reinterpret_cast<char*>(fooRvoidPint));
}

void fooRvoidPint_2(int a) {
    test::called.insert(reinterpret_cast<char*>(fooRvoidPint_2));
}
