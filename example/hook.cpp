#include <cstdio>

#include "hook.hpp"

namespace fake {
    std::unordered_map<char*, char*> subs;
}

extern "C" {

int __fake_subject_hook(void *callee) {
    printf("__fake_subject_hook; callee: %p\n", callee);
    return ::fake::subs.count(reinterpret_cast<char*>(callee)) > 0;
    //return 1;
}

void __fake_hook(void *callee) {
    printf("__fake_hook; callee: %p\n", callee);
    using sig = void();
    using funptr = sig*;
    char* fake_fun = ::fake::subs.at(reinterpret_cast<char*>(callee));
    funptr f = reinterpret_cast<funptr>(fake_fun);
    f();
}

}
