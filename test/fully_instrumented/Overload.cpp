#include "FooFixture.hpp"

struct Overload : FooFixture {};

int foo(int) { return 1; }
int foo(double) { return 2; }
int foo(int, int) { return 3; }

int fake_foo_i(int) { return 11; }
int fake_foo_d(double) { return 22; }
int fake_foo_ii(int, int) { return 33; }

TEST_F(Overload, freeFunction) {
    // using SignAlias = int(int);
    // using FunPtr = SignAlias *;
    // FunPtr funptr = __function_id foo;
    //_substitute_function((const char *)funptr,
    //(const char *)__function_id fake_foo_d);
    SUBSTITUTE(int(int), foo, fake_foo_i);
    EXPECT_EQ(foo(1), 11);
    SUBSTITUTE(int(double), foo, fake_foo_d);
    EXPECT_EQ(foo(1.0f), 22);
    SUBSTITUTE(int(int, int), foo, fake_foo_ii);
    EXPECT_EQ(foo(1, 1), 33);
}

namespace {
namespace MemberFunction{
struct Struct {
    int foo(int) { return 1; }
    int foo(double) { return 2; }
    int foo(int, int) { return 3; }
};

int Struct_fake_foo_i(Struct*, int) { return 11; }
int Struct_fake_foo_d(Struct*, double) { return 22; }
int Struct_fake_foo_ii(Struct*, int, int) { return 33; }
}}

TEST_F(Overload, MemberFunction) {
    using namespace MemberFunction;
    SUBSTITUTE(int(int), Struct::foo, Struct_fake_foo_i);
    Struct s;
    EXPECT_EQ(s.foo(1), 11);
    SUBSTITUTE(int(double), Struct::foo, Struct_fake_foo_d);
    EXPECT_EQ(s.foo(1.1f), 22);
    SUBSTITUTE(int(int, int), Struct::foo, Struct_fake_foo_ii);
    EXPECT_EQ(s.foo(1, 1), 33);
}

namespace {
namespace VirtualMemberFunction{
struct Struct {
    virtual int foo(int) { return 1; }
    virtual int foo(double) { return 2; }
    virtual int foo(int, int) { return 3; }
};

int Struct_fake_foo_i(Struct*, int) { return 11; }
int Struct_fake_foo_d(Struct*, double) { return 22; }
int Struct_fake_foo_ii(int, int) { return 33; }
}}

TEST_F(Overload, VirtualMemberFunction) {
    using namespace VirtualMemberFunction;
    SUBSTITUTE(int(int), Struct::foo, Struct_fake_foo_i);
    Struct s;
    EXPECT_EQ(s.foo(1), 11);
    SUBSTITUTE(int(double), Struct::foo, Struct_fake_foo_d);
    EXPECT_EQ(s.foo(1.1f), 22);
    SUBSTITUTE(int(int, int), Struct::foo, Struct_fake_foo_ii);
    EXPECT_EQ(s.foo(1, 1), 33);
}
