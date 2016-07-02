#include <cstdio>

extern "C" {

int __fake_subject_hook(void *callee) {
    printf("__fake_subject_hook; callee: %p\n", callee);
    return 1;
}

void __fake_hook(void *callee) {
    printf("__fake_hook; callee: %p\n", callee);
}

}
