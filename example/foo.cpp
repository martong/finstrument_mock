#include <cstdio>
void foo() {
    printf("foo called\n");
    printf("foo's address: %p\n", foo);
}

void foo2() {
    printf("foo2 called\n");
    printf("foo2's address: %p\n", foo2);
}
