#include <cstdio>
#include "foo.hpp"

#include "called.hpp"

void fooRvoidPvoid() {
    test::called.insert(reinterpret_cast<char*>(fooRvoidPvoid));
}

void fooRvoidPvoid_2() {
    test::called.insert(reinterpret_cast<char*>(fooRvoidPvoid_2));
}
