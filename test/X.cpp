#include "X.hpp"
#include "called.hpp"

// TODO remove from here
template <typename T>
char *address(T t) {
    // This is the only way to get the absolute address of a member function
    // http://stackoverflow.com/questions/8121320/get-memory-address-of-member-function
    return (char*&)(t);
}

void X::foo(int) {
    ::ftest::called.insert(address(&X::foo));
}

void X::foo_2(int) {
    ::ftest::called.insert(address(&X::foo_2));
}
