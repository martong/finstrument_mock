#include <cstdio>
#include <cstdarg>

#include "hook.hpp"

// We need to make sure that the template instantiation of the underlying map
// is happening with a unique type.
// This is must because if we'd use a static in the global namespace then
// the linker might choose an instantiation from an other TU which might had
// been compiled with -fsanitize=mock.
// That instantiation would call into __fake_hook, which in turn would call into
// the instantiation, a vicious infinit circle.
// Unnamed namespace will handle this.
namespace {

using Map = std::unordered_map<char*, char*>;
Map& subs() {
    static Map s;
    return s;
}

}

namespace fake {

void clear() { subs().clear(); }
void insert(std::pair<char*, char*> p) { subs().insert(p); }

}

extern "C" {

void* __fake_hook(void *callee) {
    //Debug
    //printf("__fake_hook; callee: %p\n", callee);
    auto it = subs().find(reinterpret_cast<char*>(callee));
    if (it == subs().end()) { return nullptr; }
    else { return it->second; }
}

} // extern "C"
