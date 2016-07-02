#include <cstdio>

extern "C" {

int __hook(void *callee) {
    printf("Hello finstrument_mock\n");
    printf("callee: %p\n", callee);
    return 1;
}

}
