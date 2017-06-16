#pragma once

#include "hook.h"
#include <cstdio>
#include <cstddef>

namespace fake {

template <typename T> const char *address(T t) {
  /// This is the only way to get the absolute address of a non-virtual
  /// member-function
  /// http://stackoverflow.com/questions/8121320/get-memory-address-of-member-function
  return (const char *&)(t);
}

template <typename Class, typename MemPtr>
const char *address_of_virtual_fun(const Class *aClass, MemPtr memptr) {
  const char **vtable = *(const char ***)aClass;

  struct pointerToMember {
    size_t pointerOrOffset;
    ptrdiff_t thisAdjustment;
  };

  pointerToMember p;
  memcpy(&p, &memptr, sizeof(p));

  static const size_t pfnAdjustment = 1;
  size_t offset = (p.pointerOrOffset - pfnAdjustment) / sizeof(char *);

  return vtable[offset];
}

} // namespace fake

template <typename T, typename U> void SUBSTITUTE(T src, U dst) {
  _substitute_function(::fake::address(src), ::fake::address(dst));
}

/// Virtual functions are problematic, because a pointer-to-member function
/// has a different layout in case of virtual functions than in case of regular
/// member functions. We have to get the address by getting the vtable from an
/// object and then getting the proper element of the vtable.
///
/// GCC has a construct with which we could get the address without knowing the
/// actually used ABI, but clang does not:
/// https://llvm.org/bugs/show_bug.cgi?id=22121
/// https://gcc.gnu.org/onlinedocs/gcc-4.9.0/gcc/Bound-member-functions.html
///
/// Note, it would be
/// better to have a compiler intrinsic which would simply return the adress of
/// the function, which is known statically during compile time.
/// Theoritically there is no need to provide an object.
///
/// We get the address now according to the Itanium C++ ABI.
/// https://mentorembedded.github.io/cxx-abi/abi.html#member-pointers
/// https://blog.mozilla.org/nfroyd/2014/02/20/finding-addresses-of-virtual-functions/
template <typename T, typename U, typename V>
void SUBSTITUTE_VIRTUAL(T src, const U &ptr_to_dummy_obj, V dst) {
  _substitute_function(
      ::fake::address_of_virtual_fun((ptr_to_dummy_obj), (src)),
      ::fake::address((dst)));
}
