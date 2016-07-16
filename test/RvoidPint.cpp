#include "called.hpp"

void fooRvoidPint(int a) {
    ::ftest::called.insert(reinterpret_cast<char*>(fooRvoidPint));
}

void fooRvoidPint_2(int a) {
    ::ftest::called.insert(reinterpret_cast<char*>(fooRvoidPint_2));
}
