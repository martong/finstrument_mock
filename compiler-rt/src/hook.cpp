//#include <cstdio>
#include <unordered_map>

#include "hook.hpp"

// We need to make sure that the template instantiation of the underlying map
// is happening with a unique type.
// This is must because if we'd use a static in the global namespace then
// the linker might choose an instantiation from an other TU which might had
// been compiled with -fsanitize=mock.
// That instantiation would call into __fake_hook, which in turn would call into
// the instantiation, a vicious infinite circle.
// Unnamed namespace will handle this.
//
// We also must build a shared library, so the linker must emit all template
// instantiations and will not chose from an other TU an other instantiation
// which might be built with -fsanitize=mock. That would again result in an
// infinite loop.
// E.g. libcpp murmur2_or_cityhash<> instantiations caused this kind of bug.
namespace {

using Map = std::unordered_map<char*, char*>;
Map& subs() {
    static Map s;
    return s;
}

} // unnamed

namespace fake {

void clear() { subs().clear(); }
void insert(char* src, char* dst) { subs().insert({src, dst}); }
}

extern "C" {

void* __fake_hook(void* callee) {
    // Debug
    // printf("__fake_hook; callee: %p\n", callee);
    auto it = subs().find(reinterpret_cast<char*>(callee));
    if (it == subs().end()) {
        return nullptr;
    } else {
        return it->second;
    }
}

} // extern "C"
