#include <cstdio>
#include <cstdarg>

#include "hook.hpp"

namespace fake {
    std::unordered_map<char*, char*> subs;
}

extern "C" {

void* __fake_hook(void *callee) {
    printf("__fake_hook; callee: %p\n", callee);
    auto it = ::fake::subs.find(reinterpret_cast<char*>(callee));
    if (it == ::fake::subs.end()) { return nullptr; }
    else { return it->second; }
}

} // extern "C"
