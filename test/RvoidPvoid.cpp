#include "called.hpp"

void fooRvoidPvoid() {
    ::ftest::called.insert(reinterpret_cast<char*>(fooRvoidPvoid));
}

void fooRvoidPvoid_2() {
    ::ftest::called.insert(reinterpret_cast<char*>(fooRvoidPvoid_2));
}
