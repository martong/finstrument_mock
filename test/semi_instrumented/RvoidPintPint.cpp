#include "called.hpp"

void RvoidPintPint(int, int) {
    ::ftest::called.insert(reinterpret_cast<char*>(RvoidPintPint));
}

void RvoidPintPint_2(int, int) {
    ::ftest::called.insert(reinterpret_cast<char*>(RvoidPintPint_2));
}

