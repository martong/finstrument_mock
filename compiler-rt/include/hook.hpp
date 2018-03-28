#pragma once

#include "hook.h"
#include <cstdio>
#include <cstddef>

//template <typename T, typename U> void __SUBSTITUTE(T src, U dst) {
  //_substitute_function((const char *)(src), (const char *)(dst));
//}

//#define SUBSTITUTE(src, dst)                                                   \
  //do {                                                                         \
    //__SUBSTITUTE(__function_id src, __function_id dst);                        \
  //} while (0)

#define SUBSTITUTE_BASE(src, dst)                                              \
  do {                                                                         \
    _substitute_function((const char *)__function_id src,                      \
                         (const char *)__function_id dst);                     \
  } while (0)

#define SUBSTITUTE_OVERLOAD(signature, src, dst)                               \
  do {                                                                         \
    using SignAlias = signature;                                               \
    using FunPtr = SignAlias *;                                                \
    FunPtr funptr = __function_id src;                                         \
    _substitute_function((const char *)funptr,                                 \
                         (const char *)__function_id dst);                     \
  } while (0)

// https://stackoverflow.com/questions/11761703/overloading-macro-on-number-of-arguments
// (Note, we could use BOOST_PP_OVERLOAD as well)
#define GET_MACRO(_1, _2, _3, NAME, ...) NAME
#define SUBSTITUTE(...)                                                        \
  GET_MACRO(__VA_ARGS__, SUBSTITUTE_OVERLOAD, SUBSTITUTE_BASE)(__VA_ARGS__)

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

//template <typename T, typename U> void SUBSTITUTE(T src, U dst) {
  //_substitute_function(::fake::address(src), ::fake::address(dst));
//}

/// Virtual functions are problematic, because a pointer-to-member function
/// has a different layout in case of virtual functions than in case of regular
/// member functions. We have to get the address by getting the vtable from an
/// object and then getting the proper element of the vtable.
///
/// GCC has a construct with which we could get the address without knowing the
/// actually used ABI, but clang does not:
/// https://llvm.org/bugs/show_bug.cgi?id=22121
/// https://gcc.gnu.org/onlinedocs/gcc-4.9.0/gcc/Bound-member-functions.html
/// The following code works (does not abort) with GCC 8.0 HEAD:
///
/// struct C {
///   virtual void foo() {}
/// };
/// struct D : C {
///   void foo() override {}
/// };
///
/// int main() {
///   C *c = new C;
///   C *d = new D;
///
///   auto abi_c_foo = address_of_virtual_fun(c, &C::foo);
///   auto abi_d_foo = address_of_virtual_fun(d, &C::foo);
///
///   typedef void (*fptr)(D *);
///   auto c_foo = (fptr)(&C::foo);
///   auto d_foo = (fptr)(&D::foo);
///
///   assert((void *)abi_c_foo == (void *)c_foo);
///   assert((void *)abi_d_foo == (void *)d_foo);
/// }
///
///
/// Note, it would be
/// better to have a compiler intrinsic which would simply return the adress of
/// the function, which is known statically during compile time.
/// Theoritically there is no need to provide an object.
///
/// We get the address now according to the Itanium C++ ABI.
/// https://mentorembedded.github.io/cxx-abi/abi.html#member-pointers
/// https://blog.mozilla.org/nfroyd/2014/02/20/finding-addresses-of-virtual-functions/

// TODO Remove
template <typename T, typename U, typename V>
//[[ deprecated ]]
void SUBSTITUTE_VIRTUAL(T src, const U &ptr_to_dummy_obj, V dst) {
  _substitute_function(
      ::fake::address_of_virtual_fun((ptr_to_dummy_obj), (src)),
      ::fake::address((dst)));
}
