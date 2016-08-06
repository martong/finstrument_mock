#include "called.hpp"

_Complex double RComplexPvoid() {
    ::ftest::called.insert(reinterpret_cast<char*>(RComplexPvoid));
    return {1,3};
}
