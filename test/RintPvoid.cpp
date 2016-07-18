#include "called.hpp"

int RintPvoid() {
    ::ftest::called.insert(reinterpret_cast<char*>(RintPvoid));
    return 13;
}

int RintPvoid_2() {
    ::ftest::called.insert(reinterpret_cast<char*>(RintPvoid_2));
    return 14;
}
