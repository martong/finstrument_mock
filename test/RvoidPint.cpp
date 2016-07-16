#include "called.hpp"

void RvoidPint(int a) {
    ::ftest::called.insert(reinterpret_cast<char*>(RvoidPint));
}

void RvoidPint_2(int a) {
    ::ftest::called.insert(reinterpret_cast<char*>(RvoidPint_2));
}
