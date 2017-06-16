#include <hook.h>
#include <stdlib.h>
#include <stdio.h>

#define ASSERT(CONDITION)                                                      \
    do                                                                         \
        if (!(CONDITION)) {                                                    \
            printf("Assertion failure %s:%d ASSERT(%s)\n", __FILE__, __LINE__, \
                   #CONDITION);                                                \
            abort();                                                           \
        }                                                                      \
    while (0)

int foo(int p) { return p * 2; }
int fake_foo(int p) { return p * 4; }

void test1() {
    ASSERT(foo(2) == 4);
    _substitute_function((const char*)foo, (const char*)fake_foo);
    ASSERT(foo(2) == 8);
}

int main() {
    test1();
    printf("OK\n");
    return 0;
}
