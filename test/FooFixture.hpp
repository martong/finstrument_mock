#pragma once

#include <gtest/gtest.h>

// The user should not need to handle varargs
// Later, a macro will generate the functions that use varargs
#include <cstdarg>

#include "hook.hpp"

#include "called.hpp"

//template <typename T>
//char *address(T t) {
    //return reinterpret_cast<char *>(t);
//}

template <typename T>
char *address(T t) {
    // This is the only way to get the absolute address of a member function
    // http://stackoverflow.com/questions/8121320/get-memory-address-of-member-function
    return (char*&)(t);
}

//template <typename T>
//char* address_of_mem_fun(T t) {
    //// http://stackoverflow.com/questions/8121320/get-memory-address-of-member-function
    //return (char*&)(t);
//}

struct FooFixture : ::testing::Test {
    FooFixture() {
    ::ftest::called.clear();
    ::fake::subs.clear();
    }
};
