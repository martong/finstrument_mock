#include <cstdio>

extern "C" {

int __fake_subject_hook(void *callee) {
    printf("Hello finstrument_mock\n");
    printf("callee: %p\n", callee);
    return 1;
}

}
