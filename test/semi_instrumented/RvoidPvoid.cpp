#include "called.hpp"

void RvoidPvoid() {
    ::ftest::called.insert(reinterpret_cast<char*>(RvoidPvoid));
}

void RvoidPvoid_2() {
    ::ftest::called.insert(reinterpret_cast<char*>(RvoidPvoid_2));
}
