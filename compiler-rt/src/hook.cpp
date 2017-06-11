#include <errno.h>
#include <unistd.h> // _SC_PAGESIZE
#include <cassert>
#include <sys/mman.h> // mmap

#include "hook.hpp"

namespace {

using u64 = unsigned long long;
using uptr = unsigned long long;
static_assert(sizeof(u64) == 8, "");
static_assert(sizeof(uptr) == 8, "");

static const u64 N = 16;

static const u64 kLinuxLowMemEnd = 0x00007fff7fff;
static const u64 kLinuxHighMemBeg = 0x7f0000000000;
static const u64 kLinuxHighMemEnd = 0x7fffffffffff;
static const u64 kLinuxLowShadowOffset = 0x00007fff80000;

static const u64 kOSXLowMemEnd = (1ull << 44) / N - 1;
static const u64 kOSXHighMemBeg = 0x7f0000000000;
static const u64 kOSXHighMemEnd = 0x7fffffffffff;
static const u64 kOSXLowShadowOffset = kOSXLowMemEnd + 1;

#define kLowMemBeg 0

#ifdef __linux__
#define kLowMemEnd kLinuxLowMemEnd
#define kHighMemBeg kLinuxHighMemBeg
#define kHighMemEnd kLinuxHighMemEnd
#define LOW_SHADOW_OFFSET kLinuxLowShadowOffset
#elif __APPLE__
#define kLowMemEnd kOSXLowMemEnd
#define kHighMemBeg kOSXHighMemBeg
#define kHighMemEnd kOSXHighMemEnd
#define LOW_SHADOW_OFFSET kOSXLowShadowOffset
#endif

#define ADDR_IS_IN_LOW_MEM(mem) ((mem) <= kLowMemEnd)

#define LOW_MEM_TO_SHADOW(mem) ((mem)*N + LOW_SHADOW_OFFSET)
#define HIGH_SHADOW_OFFSET LOW_MEM_TO_SHADOW(kLowMemEnd + 1)
#define HIGH_MEM_TO_SHADOW(mem) (((mem)-kHighMemBeg) * N + HIGH_SHADOW_OFFSET)
#define MEM_TO_SHADOW(mem)                                                     \
  (ADDR_IS_IN_LOW_MEM((mem)) ? LOW_MEM_TO_SHADOW((mem))                        \
                             : HIGH_MEM_TO_SHADOW((mem)))

#define kLowShadowBeg MEM_TO_SHADOW(kLowMemBeg)
#define kLowShadowEnd (LOW_MEM_TO_SHADOW(kLowMemEnd + 1) - 1)
#define kHighShadowBeg MEM_TO_SHADOW(kHighMemBeg)
#define kHighShadowEnd (MEM_TO_SHADOW(kHighMemEnd + 1) - 1)

void PrintAddressSpaceLayout() {
  printf("|| `[%p, %p]` || HighMem    ||\n", (void *)kHighMemBeg,
         (void *)kHighMemEnd);
  printf("|| `[%p, %p]` || HighShadow ||\n", (void *)kHighShadowBeg,
         (void *)kHighShadowEnd);
  printf("|| `[%p, %p]` || LowShadow  ||\n", (void *)kLowShadowBeg,
         (void *)kLowShadowEnd);
  printf("|| `[%p, %p]` || LowMem     ||\n", (void *)kLowMemBeg,
         (void *)kLowMemEnd);
}

uptr GetPageSize() { return sysconf(_SC_PAGESIZE); }

inline bool IsPowerOfTwo(uptr x) { return (x & (x - 1)) == 0; }

inline uptr RoundUpTo(uptr size, uptr boundary) {
  assert(IsPowerOfTwo(boundary));
  return (size + boundary - 1) & ~(boundary - 1);
}

uptr internal_mmap(void *addr, size_t length, int prot, int flags, int fd,
                   u64 offset) {
  return (uptr)mmap(addr, length, prot, flags, fd, offset);
}

int internal_munmap(void *addr, size_t length) { return munmap(addr, length); }

bool internal_iserror(uptr retval, int *rverrno) {
  if (retval == (uptr)-1) {
    if (rverrno)
      *rverrno = errno;
    return true;
  } else {
    return false;
  }
}

int Munmap(uptr fixed_addr, uptr size) {
  uptr PageSize = GetPageSize();
  int res = internal_munmap((void *)(fixed_addr & ~(PageSize - 1)),
                            RoundUpTo(size, PageSize));
  if (res != 0)
    printf("ERROR: %s failed to "
           "deallocate 0x%llx (%zd) bytes at address %llx (errno: %d)\n",
           "MockSanitizer", size, size, fixed_addr, res);
  return res;
}

void *MmapFixedNoReserve(uptr fixed_addr, uptr size, const char *name) {
  int fd = -1;
  unsigned flags = MAP_PRIVATE | MAP_FIXED | MAP_NORESERVE;
  if (fd == -1)
    flags |= MAP_ANON;

  uptr PageSize = GetPageSize();
  uptr p = internal_mmap((void *)(fixed_addr & ~(PageSize - 1)),
                         RoundUpTo(size, PageSize), PROT_READ | PROT_WRITE,
                         flags, fd, 0);
  int reserrno;
  if (internal_iserror(p, &reserrno))
    printf("ERROR: %s failed to "
           "allocate 0x%llx (%zd) bytes at address %llx (errno: %d)\n",
           "MockSanitizer", size, size, fixed_addr, reserrno);
  return (void *)p;
}

#define CHECK_IMPL(c1, op, c2)                                                 \
  do {                                                                         \
    auto v1 = (u64)(c1);                                                       \
    auto v2 = (u64)(c2);                                                       \
    assert(v1 op v2);                                                          \
  } while (false)

#define CHECK(a) CHECK_IMPL((a), !=, 0)
#define CHECK_EQ(a, b) CHECK_IMPL((a), ==, (b))
#define CHECK_NE(a, b) CHECK_IMPL((a), !=, (b))
#define CHECK_LT(a, b) CHECK_IMPL((a), <, (b))
#define CHECK_LE(a, b) CHECK_IMPL((a), <=, (b))
#define CHECK_GT(a, b) CHECK_IMPL((a), >, (b))
#define CHECK_GE(a, b) CHECK_IMPL((a), >=, (b))

uptr GetMmapGranularity() { return GetPageSize(); }

void ReserveShadowMemoryRange(uptr beg, uptr end, const char *name) {
  CHECK_EQ((beg % GetMmapGranularity()), 0);
  CHECK_EQ(((end + 1) % GetMmapGranularity()), 0);
  uptr size = end - beg + 1;
  void *res = MmapFixedNoReserve(beg, size, name);
  if (res != (void *)beg) {
    printf("ReserveShadowMemoryRange failed while trying to map 0x%llx bytes. "
           "Perhaps you're using ulimit -v\n",
           size);
    abort();
  }
}

void ReleaseShadowMemoryRange(uptr beg, uptr end) {
  uptr size = end - beg + 1;
  Munmap(beg, size);
}

__attribute__((constructor)) void mocksan_init() {
  printf("mocksan_init\n");
  PrintAddressSpaceLayout();
  ReserveShadowMemoryRange(kLowShadowBeg, kLowShadowEnd, "low shadow");
  ReserveShadowMemoryRange(kHighShadowBeg, kHighShadowEnd, "high shadow");
}

inline uptr MemToShadow(uptr p) { return MEM_TO_SHADOW(p); }

inline bool AddrIsInLowMem(uptr a) { return a < kLowMemEnd; }

inline bool AddrIsInHighMem(uptr a) {
  return a >= kHighMemBeg && a <= kHighMemEnd;
}

inline bool AddrIsInMem(uptr a) {
  return AddrIsInLowMem(a) || AddrIsInHighMem(a);
}

} // unnamed

namespace fake {

void clear() {
  printf("mocksan clear()\n");
  ReleaseShadowMemoryRange(kLowShadowBeg, kLowShadowEnd);
  ReleaseShadowMemoryRange(kHighShadowBeg, kHighShadowEnd);
  ReserveShadowMemoryRange(kLowShadowBeg, kLowShadowEnd, "low shadow");
  ReserveShadowMemoryRange(kHighShadowBeg, kHighShadowEnd, "high shadow");
}

void insert(char *src, char *dst) {
  printf("insert; src, dst: %p, %p\n", src, dst);
  assert(AddrIsInMem((uptr)src));
  uptr shadow = MemToShadow((uptr)src);
  char **shadowPtr = reinterpret_cast<char **>(shadow);
  printf("insert; shadowPtr: %p\n", shadowPtr);
  *shadowPtr = dst;
}

} // fake

extern "C" {

void *__fake_hook(void *callee) {
  // Debug
  printf("__fake_hook; callee: %p\n", callee);
  assert(AddrIsInMem((uptr)callee));

  uptr shadow = MemToShadow((uptr)callee);
  char **shadowPtr = reinterpret_cast<char **>(shadow);
  printf("__fake_hook; shadow: %p\n", shadowPtr);
  char *shadowValue = *shadowPtr;
  printf("__fake_hook; shadowValue: %p\n", shadowValue);
  // assert(shadowValue == nullptr);

  return shadowValue;
}

} // extern "C"
