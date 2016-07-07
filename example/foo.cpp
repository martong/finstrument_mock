#include <cstdio>
#include "foo.hpp"

#include "called.hpp"

void foo() {
    test::called.insert(reinterpret_cast<char*>(foo));
    printf("foo called\n");
    printf("foo's address: %p\n", foo);
}

void foo2() {
    test::called.insert(reinterpret_cast<char*>(foo2));
    printf("foo2 called\n");
    printf("foo2's address: %p\n", foo2);
}
