#include <cstdio>
#include <cstdarg>

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

void* __fake_hook_map(void *callee) {
    printf("__fake_hook; callee: %p\n", callee);
    char* fake_fun = ::fake::subs.at(reinterpret_cast<char*>(callee));
    return fake_fun;
}

} // extern "C"
