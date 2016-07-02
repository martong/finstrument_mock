#include "hook.hpp"

void foo();
void foo2();
void fake_foo() {
    printf("fake_foo called\n");
}

// bar calls foo and foo2,
// we want to fake foo, but not foo2
void bar();

int main() {

    ::fake::subs.insert({reinterpret_cast<char*>(foo), reinterpret_cast<char*>(fake_foo)});

    bar();
    return 0;
}
