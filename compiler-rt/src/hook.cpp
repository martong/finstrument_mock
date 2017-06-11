//#include <cstdio>
#include <unistd.h> // _SC_PAGESIZE
#include <cassert>
#include <sys/mman.h> // mmap
#include <unordered_map>

#include "hook.hpp"


using u64 = unsigned long long;
using uptr = unsigned long;
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
#define HIGH_SHADOW_OFFSET LOW_MEM_TO_SHADOW(kLowMemEnd + 1)
#define HIGH_MEM_TO_SHADOW(mem) (((mem) - kHighMemBeg) * N + HIGH_SHADOW_OFFSET)
#define MEM_TO_SHADOW(mem) (LOW_ADDR((mem)) ? LOW_MEM_TO_SHADOW((mem)) : HIGH_MEM_TO_SHADOW((mem)))

#define kLowShadowBeg   MEM_TO_SHADOW(kLowMemBeg)
#define kLowShadowEnd   (LOW_MEM_TO_SHADOW(kLowMemEnd + 1) - 1)
#define kHighShadowBeg  MEM_TO_SHADOW(kHighMemBeg)
#define kHighShadowEnd  (MEM_TO_SHADOW(kHighMemEnd + 1) - 1)

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

uptr GetPageSize() {
  return sysconf(_SC_PAGESIZE);
}

inline bool IsPowerOfTwo(uptr x) {
  return (x & (x - 1)) == 0;
}

inline uptr RoundUpTo(uptr size, uptr boundary) {
  assert(IsPowerOfTwo(boundary));
  return (size + boundary - 1) & ~(boundary - 1);
}

uptr internal_mmap(void *addr, size_t length, int prot, int flags,
                   int fd, u64 offset) {
  return (uptr)mmap(addr, length, prot, flags, fd, offset);
}

bool internal_iserror(uptr retval, int *rverrno) {
  if (retval == (uptr)-1) {
    if (rverrno)
      *rverrno = errno;
    return true;
  } else {
    return false;
  }
}

void *MmapFixedNoReserve(uptr fixed_addr, uptr size, const char *name) {
  int fd = -1;
  unsigned flags = MAP_PRIVATE | MAP_FIXED | MAP_NORESERVE;
  if (fd == -1) flags |= MAP_ANON;

  uptr PageSize = GetPageSize();
  uptr p = internal_mmap((void *)(fixed_addr & ~(PageSize - 1)),
                         RoundUpTo(size, PageSize), PROT_READ | PROT_WRITE,
                         flags, fd, 0);
  int reserrno;
  if (internal_iserror(p, &reserrno))
    printf("ERROR: %s failed to "
           "allocate 0x%zx (%zd) bytes at address %zx (errno: %d)\n",
           "MockSanitizer", size, size, fixed_addr, reserrno);
  return (void *)p;
}

#define CHECK_IMPL(c1, op, c2) \
  do { \
    auto v1 = (u64)(c1); \
    auto v2 = (u64)(c2); \
    assert(v1 op v2); \
  } while (false) \

#define CHECK(a)       CHECK_IMPL((a), !=, 0)
#define CHECK_EQ(a, b) CHECK_IMPL((a), ==, (b))
#define CHECK_NE(a, b) CHECK_IMPL((a), !=, (b))
#define CHECK_LT(a, b) CHECK_IMPL((a), <,  (b))
#define CHECK_LE(a, b) CHECK_IMPL((a), <=, (b))
#define CHECK_GT(a, b) CHECK_IMPL((a), >,  (b))
#define CHECK_GE(a, b) CHECK_IMPL((a), >=, (b))

uptr GetMmapGranularity() {
  return GetPageSize();
}

void ReserveShadowMemoryRange(uptr beg, uptr end, const char *name) {
  CHECK_EQ((beg % GetMmapGranularity()), 0);
  CHECK_EQ(((end + 1) % GetMmapGranularity()), 0);
  uptr size = end - beg + 1;
  void *res = MmapFixedNoReserve(beg, size, name);
  if (res != (void*)beg) {
    printf("ReserveShadowMemoryRange failed while trying to map 0x%zx bytes. "
           "Perhaps you're using ulimit -v\n", size);
    abort();
  }
}

__attribute__((constructor))
void mocksan_init() {
    printf("mocksan_init\n");
    PrintAddressSpaceLayout();
    ReserveShadowMemoryRange(kLowShadowBeg, kLowShadowEnd, "low shadow");
    ReserveShadowMemoryRange(kHighShadowBeg, kHighShadowEnd, "high shadow");
}

inline uptr MemToShadow(uptr p) {
  return MEM_TO_SHADOW(p);
}

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
    printf("__fake_hook; callee: %p\n", callee);

    uptr shadow = MemToShadow((uptr)callee);
    char* shadowPtr = reinterpret_cast<char*>(shadow);
    printf("__fake_hook; shadow: %p\n", shadowPtr);
    char* shadowValue = reinterpret_cast<char*>(*shadowPtr);
    printf("__fake_hook; shadowValue: %p\n", shadowValue);
    assert(shadowValue == nullptr);

    auto it = subs().find(reinterpret_cast<char*>(callee));
    if (it == subs().end()) {
        return nullptr;
    } else {
        return it->second;
    }
}

} // extern "C"
