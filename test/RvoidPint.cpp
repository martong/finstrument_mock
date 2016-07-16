#include "called.hpp"

void RvoidPint(int a) {
    ::ftest::called.insert(reinterpret_cast<char*>(RvoidPint));
}

void RvoidPint_2(int a) {
    ::ftest::called.insert(reinterpret_cast<char*>(RvoidPint_2));
}

void RvoidPint_3(int a) {
    ::ftest::called.insert(reinterpret_cast<char*>(RvoidPint_3));
}

void RvoidPint_4(int a) {
    ::ftest::called.insert(reinterpret_cast<char*>(RvoidPint_4));
}
