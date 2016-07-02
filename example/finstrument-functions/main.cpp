#include <stdio.h>

void foo() { printf("in foo[%p]\n", foo); }

void bar() {
    printf("in bar[%p]\n", bar);
    foo();
}

int main() {
    printf("in main[%p]\n", main);
    bar();
    return 0;
}
