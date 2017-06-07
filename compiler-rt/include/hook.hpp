#pragma once

#include <cstdio>
#include <cstddef>

namespace fake {

void clear();
void insert(char* src, char* dst);

template <typename T>
char* address(T t) {
    /// This is the only way to get the absolute address of a non-virtual
    /// member-function
    /// http://stackoverflow.com/questions/8121320/get-memory-address-of-member-function
    return (char*&)(t);
}

template <typename Class, typename MemPtr>
char* address_of_virtual_fun(Class* aClass, MemPtr memptr) {
    char** vtable = *(char***)aClass;

    struct pointerToMember {
        size_t pointerOrOffset;
        ptrdiff_t thisAdjustment;
    };

    pointerToMember p;
    memcpy(&p, &memptr, sizeof(p));

    static const size_t pfnAdjustment = 1;
    size_t offset = (p.pointerOrOffset - pfnAdjustment) / sizeof(char*);

    return vtable[offset];
}

} // namespace fake

// TODO use functions (?)
#define SUBSTITUTE(src, dst)                                            \
    do {                                                                \
        ::fake::insert(::fake::address((src)), ::fake::address((dst))); \
    } while (0)

/// Use this macro for substituting noreturn functions.
///
/// Note, be warned, you are going to have UB:
/// The standard states: The behavior is undefined if the function with this
/// attribute actually returns.
///
/// Note2, for some reason
/// SUBSTITUTE(&std::exit, &fake_exit) fails because of a strange linker error.
///   ../compiler-rt/include/hook.hpp:9:7: error: definition with same mangled
///   name as another definition
///   char *address(T t) {
///         ^
///   ../compiler-rt/include/hook.hpp:9:7: note: previous definition is here
///   1 error generated.
///
///   This is happening always with __dead2 (noreturn) functions in stdlib.h
///
///   Bug:
///   https://llvm.org/bugs/show_bug.cgi?id=28909
#define SUBSTITUTE_NORETURN(src, dst)                   \
    do {                                                \
        ::fake::insert((char*)((src)), (char*)((dst))); \
    } while (0)

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
#define SUBSTITUTE_VIRTUAL(src, ptr_to_dummy_obj, dst)                 \
    do {                                                               \
        ::fake::insert(                                                \
            ::fake::address_of_virtual_fun((ptr_to_dummy_obj), (src)), \
            ::fake::address((dst)));                                   \
    } while (0)
