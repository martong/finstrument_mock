#include "X.hpp"
#include "called.hpp"
#include "address.hpp"
#include "hook.hpp"

void X::foo(int) {
    ::ftest::called.insert(address(&X::foo));
}

void X::foo_2(int) {
    ::ftest::called.insert(address(&X::foo_2));
}
