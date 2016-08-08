#pragma once

namespace fake {

void clear();
void insert(char* src, char* dst);

template <typename T>
char* address(T t) {
    /// This is the only way to get the absolute address of a member function
    /// http://stackoverflow.com/questions/8121320/get-memory-address-of-member-function
    return (char*&)(t);
}

} // namespace fake

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
