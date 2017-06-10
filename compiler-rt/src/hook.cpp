//#include <cstdio>
#include <unordered_map>

#include "hook.hpp"

namespace {

using u64 = unsigned long long;
static const u64 kLinuxLowMemEnd = 0x00007fff7fff;
static const u64 kLinuxHighMemBeg = 0x7f0000000000;
static const u64 kLinuxHighMemEnd = 0x7fffffffffff;
static const u64 N = 16;
static const u64 kLinuxLowShadowOffset = 0x00007fff80000;
//static const u64 kLinuxHighShadowOffset = LinuxLowShadowOffset * (N) + 1;

#define kLowMemBeg      0
#ifdef __linux__
  #define kLowMemEnd kLinuxLowMemEnd
  #define kHighMemBeg kLinuxHighMemBeg
  #define kHighMemEnd kLinuxHighMemEnd

  #define LOW_SHADOW_OFFSET kLinuxLowShadowOffset
#elif __APPLE__
#endif

// mem & 0xffff00000000 == 0
#define LOW_ADDR(mem) ((mem) <= kLowMemEnd)

#define LOW_MEM_TO_SHADOW(mem) ((mem) * N + LOW_SHADOW_OFFSET)
#define HIGH_SHADOW_OFFSET LOW_MEM_TO_SHADOW(kLowMemEnd) + 1
#define HIGH_MEM_TO_SHADOW(mem) (((mem) - kHighMemBeg) * N + HIGH_SHADOW_OFFSET)
#define MEM_TO_SHADOW(mem) (LOW_ADDR((mem)) ? LOW_MEM_TO_SHADOW((mem)) : HIGH_MEM_TO_SHADOW((mem)))

#define kLowShadowBeg   MEM_TO_SHADOW(kLowMemBeg)
#define kLowShadowEnd   MEM_TO_SHADOW(kLowMemEnd)
#define kHighShadowBeg  MEM_TO_SHADOW(kHighMemBeg)
#define kHighShadowEnd  MEM_TO_SHADOW(kHighMemEnd)

void PrintAddressSpaceLayout() {
  printf("|| `[%p, %p]` || HighMem    ||\n",
         (void*)kHighMemBeg, (void*)kHighMemEnd);
  printf("|| `[%p, %p]` || HighShadow ||\n",
         (void*)kHighShadowBeg, (void*)kHighShadowEnd);
  printf("|| `[%p, %p]` || LowShadow  ||\n",
         (void*)kLowShadowBeg, (void*)kLowShadowEnd);
  printf("|| `[%p, %p]` || LowMem     ||\n",
         (void*)kLowMemBeg, (void*)kLowMemEnd);
}

__attribute__((constructor))
void mocksan_init() {
    printf("mocksan_init\n");
    PrintAddressSpaceLayout();
}


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
    printf("__fake_hook; callee: %p\n", callee);
    auto it = subs().find(reinterpret_cast<char*>(callee));
    if (it == subs().end()) {
        return nullptr;
    } else {
        return it->second;
    }
}

} // extern "C"
