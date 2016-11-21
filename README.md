# Instrumentation for testing

* [Introduction](#introduction)
   * [Alternatives](#alternatives)
      * [Link Seams](#link-seams)
      * [Preprocessor Seams](#preprocessor-seams)
      * [Object and Compile Seams](#object-and-compile-seams)
      * [Isolator  ](#isolator)
* [Motivating Examples](#motivating-examples)
   * [Replace template functions](#replace-template-functions)
   * [Replace functions in class templates](#replace-functions-in-class-templates)
   * [Replace (always inline) functions in STL](#replace-always-inline-functions-in-stl)
   * [Replace calls in libc - fopen(), fread()](#replace-calls-in-libc---fopen-fread)
   * [Replace system calls - time()](#replace-system-calls---time)
   * [Eliminate death tests, replace [[noreturn]] functions](#eliminate-death-tests-replace-noreturn-functions)
* [How does it work?](#how-does-it-work)
   * [Constexpr functions](#constexpr-functions)
   * [Virtual functions](#virtual-functions)
* [How to build](#how-to-build)
* [Usage](#usage)
   * [Virtual functions](#virtual-functions-1)
      * [Replace virtual functions](#replace-virtual-functions)
   * [[[noreturn]] functions](#noreturn-functions)
   * [Difficulties with libcxx and always_inline attribute](#difficulties-with-libcxx-and-always_inline-attribute)
   * [How to access privates?](#how-to-access-privates)
* [Performance](#performance)
* [Future work](#future-work)
   * [Compile time reflection](#compile-time-reflection)
* [Outlook](#outlook)
* [Acknowledgement](#acknowledgement)

## Introduction
There are many legacy enterprise applications that were written without automated unit tests. It is often very difficult to maintain and modify such code, since we cannot verify the changes. A frequently used approach in this situation is to write additional tests without modifying the original source code. There are several techniques to do this ([Alternatives](#alternatives)). However, these techniques have their own limitations and disadvantages.

My aim is to provide an alternative technique without those limitations. I used the Clang compiler sanitizer infrastructure to implement testing specific instrumentation (prototype). The instrumentation makes it possible to replace any C/C++ function with a corresponding test double function.

### Alternatives
Currently in C/C++ we have techniques to test without modifying the original source code: using a seam.
A *seam* is an abstract concept introduced by Feathers [1] as an instrument via we can alter the behaviour of our unit without changing its source code.
There are four different kind of seams in C++ [2]:
* Link seam: Change the definition of a function via some linker specific setup.
* Preprocessor seam: With the help of the preprocessor, redefine function names to use an alternative implementation.
* Object seam: Based on inheritance to inject a subclass with an alternative implementation.
* Compile seam: Inject dependencies at compile-time through template parameters.

The *enabling point* of a seam is the place where we can make the decision to use one behavior or another.
Different seams have different enabling points.

[1] Michael Feathers, Working Effectively with Legacy Code, 2004<br/>
[2] https://accu.org/index.php/journals/1927

#### Link Seams
We can use a link seam, e.g. to replace the implementation of a free function or a member function.
For instance:
```c++
// A.hpp
void foo();
// A.cpp
void foo() { ... };
// MockA.cpp
void foo() { ... };
// B.cpp
#include "A.hpp"
void bar() { foo(); ... }
```
In one hand, when we would test the `bar()` function then we would link the test executable with the `MockA.o` object file.
On the other hand, we would link the production code with `A.o`.
More information on how to use link seams in practice:
* http://jayconrod.com/posts/23/tutorial-function-interposition-in-linux
* http://samanbarghi.com/blog/2014/09/05/how-to-wrap-a-system-call-libc-function-in-linux/
* http://stackoverflow.com/questions/31131163/how-to-hook-all-linux-system-calls-during-a-binary-execution

Link-time dependency replacement is not possible if the dependency is defined in a static library or in the same translation unit where the SUT is defined.
It is also not feasible to use link seams if the dependency is implemented as an inline function.
This makes the use of this seam cumbersome or impossible when the dependant unit is a template or when the dependency is a template.
The enabling point for a link seam is always outside of the program text.
This makes the use of link seams somewhat hard to notice.
On top of all, link-time substitution requires strong support from the build system we are using.
Thus, we might have to specialize the building of the tests for each and every unit.
This does not scale well and can be really demanding regarding to maintenance.

As a summary, the major limitations of link seams are that we cannot use them with:
* static libraries
* header only libraries
* inline functions
* templates

#### Preprocessor Seams
Preprocessor seams can be applied to replace the invocation of a global function to an invocation of a test double.
Let's consider the following code snippet:
```c++
void *my_malloc(size_t size) {
  //...
  return malloc(size);
}

void my_free(void *p) {
  //...
  return free(p);
}

#define free my_free
#define malloc my_malloc

void unitUnderTest() {
  int *array = (int *)malloc(4 * sizeof(int));
  // do something with array
  free(array);
}
```
We can replace the standard `malloc()` and `free()` functions with our own implementation.
One example usage can be to collect statistics or do sanity checks in `my_malloc` and `my_free` functions.
These seams can be applied conveniently in C, but not in C++.
As soon as we are using namespaces, then the preprocessor might generate code which cannot be compiled because of ambiguous use of names.
Dangerous side effects of macros are also well-known.

#### Object and Compile Seams

Object/compile seams are realized by introducing a runtime/compile-time interface.
If we are lucky, then the unit we want to test depends only on the interface.
In this case we can provide an alternative implementation for the test.

However, in legacy code it is quite often that the dependency is hardwired to a specific implementation.
In these cases, using an object or compile seam would require instrusive changes in the original source code (e.g. adding a new interface and a setter/constructor).
Consequently, there are numerous situations when we cannot use object and compile seams.

#### Isolator++
With Isolator++ one can create non-intrusive tests on Windows, but Windows only.
https://www.typemock.com/isolatorpp-product-page
The project is closed source and commercial, therefore we cannot know what technology (or seam) they use.
Perhaps they use runtime instrumentation to alter the behaviour of the unit.

## Motivating Examples
The below examples are actually working code extracts of the tests from this repository.

### Replace template functions
```c++
// unit_under_test.hpp
template <typename T>
T FunTemp(T t) {
    return t;
}
inline int foo(int p) {
    return FunTemp(p);
}

// test.cpp
TEST_F(FooFixture, CallFunT) {
    SUBSTITUTE(&FunTemp<int>, &fake_FunTemp);
    int p = 13;
    auto res = foo(p);
    EXPECT_EQ(res, 39);
}
```

### Replace functions in class templates
```c++
// unit_under_test.hpp
template <typename T>
struct TemplateS {
    int foo(int p) { return bar(p); }
    int bar(int p) { return p; }
};

// test.cpp
int fake_bar_mem_fun(TemplateS<int>* self, int p) { return p * 3; }
TEST_F(FooFixture, ClassT) {
    SUBSTITUTE(&TemplateS<int>::bar, &fake_bar_mem_fun);
    TemplateS<int> t;
    auto res = t.foo(13);
    EXPECT_EQ(res, 39);
}
```

### Replace (always inline) functions in STL
Consider the following concurrent `Entity`:
```c++
// Entity.hpp

#include <mutex>
#include <numeric>
#include <vector>

class Entity {
public:
    int process(int i) const {
        std::unique_lock<std::mutex> lock{m, std::try_to_lock};
        if (lock.owns_lock()) {
            auto result = std::accumulate(v.begin(), v.end(), i);
            return result;
        } else {
            return -1;
        }
        return 0;
    }
    void add(int i) {
        std::lock_guard<std::mutex> lock{m};
        v.push_back(i);
    }

private:
    std::vector<int> v;
    mutable std::mutex m;
};
```
We can test the behaviour when the `mutex` is not locked:
```c++
// test.cpp

#include "FooFixture.hpp"

#include "Entity.hpp"

namespace {

bool owns_lock_result;
using Lock = std::unique_lock<std::mutex>;
bool fake_owns_lock(Lock*) { return owns_lock_result; }

} // unnamed

TEST_F(FooFixture, Mutex2) {
    SUBSTITUTE(&Lock::owns_lock, &fake_owns_lock);
    Entity e;
    owns_lock_result = false;
    EXPECT_EQ(e.process(1), -1);
    owns_lock_result = true;
    EXPECT_EQ(e.process(1), 1);
}
```

### Replace calls in libc - fopen(), fread()
```c++
// Fread.hpp
#pragma once

struct FeofE {};
struct FerrorE {};

enum { SIZE = 5 };
double Fread(void);
```

```c++
// Fread.cpp

double Fread(void) {
    double b[SIZE];
    FILE* fp = fopen("test.bin", "rb");
    if (!fp) {
        throw FerrorE{};
    }
    size_t ret_code =
        fread(b, sizeof *b, SIZE, fp); // reads an array of doubles
    double result = 0.0;
    if (ret_code == SIZE) {
        puts("Array read successfully, contents: ");
        for (int n = 0; n < SIZE; ++n) {
            printf("%f ", b[n]);
            result += b[n];
        }
        putchar('\n');
    } else { // error handling
        if (feof(fp)) {
            printf("Error reading test.bin: unexpected end of file\n");
            throw FeofE{};
        } else if (ferror(fp)) {
            perror("Error reading test.bin");
            throw FerrorE{};
        }
    }

    fclose(fp);
    return result;
}
```

```c++
// Test.cpp

#include <stdio.h>
#include <cassert>
#include <array>
#include <numeric>

#include "FooFixture.hpp"
#include "Fread.hpp"

namespace t1 {
FILE f;

FILE *fake_fopen(const char *, const char *) { return &f; }

std::array<double, SIZE> a = {1., 2., 3., 4., 5.};
size_t fake_fread(void *buf, size_t, size_t count, FILE *) {
    assert(count == SIZE);
    double *b = reinterpret_cast<double *>(buf);
    for (int i = 0; i < 5; ++i) {
        b[i] = a[i];
    }
    return count;
}

int fake_feof(FILE *) { return 0; }

int fake_ferror(FILE *) { return 0; }

int fake_fclose(FILE *) { return 0; }
} // namespace t1

TEST_F(FooFixture, FreadSumIsOk) {
    SUBSTITUTE(&fopen, &t1::fake_fopen);
    SUBSTITUTE(&fread, &t1::fake_fread);
    SUBSTITUTE(&feof, &t1::fake_feof);
    SUBSTITUTE(&ferror, &t1::fake_ferror);
    SUBSTITUTE(&fclose, &t1::fake_fclose);

    auto res = Fread();
    EXPECT_EQ(res, std::accumulate(t1::a.begin(), t1::a.end(), 0.0));
}


namespace t2 {
FILE f;

FILE *fake_fopen(const char *, const char *) { return &f; }

size_t fake_fread(void *buf, size_t, size_t count, FILE *) {
    return 0;
}

int fake_feof(FILE *) { return 1; } // indicate EOF !

int fake_ferror(FILE *) { return 0; }

int fake_fclose(FILE *) { return 0; }
} // namespace t2

TEST_F(FooFixture, FreadHandles_feof) {
    SUBSTITUTE(&fopen, &t2::fake_fopen);
    SUBSTITUTE(&fread, &t2::fake_fread);
    SUBSTITUTE(&feof, &t2::fake_feof);
    SUBSTITUTE(&ferror, &t2::fake_ferror);
    SUBSTITUTE(&fclose, &t2::fake_fclose);

    EXPECT_THROW(Fread(), FeofE);
}
```

### Replace system calls - time()
```c++
struct Message {
    int type;
    std::vector<int> events;
};

void reloadConfig() { /*...*/ }
void processMessage(int msgType) { /*...*/ }
void processEvent(int event) { /*...*/ }

#define REALOAD_TIMEOUT 60

void handleMessage(const Message& msg) {
    time_t start = time(NULL);
    processMessage(msg.type);
    for (int i = 0; i < msg.events.size(); ++i) {
        processEvent(msg.events[i]);
    }
    time_t duration = (time(NULL) - start);
    if (duration > REALOAD_TIMEOUT) {
        reloadConfig();
    }
}


// TEST.cpp
#include <gtest/gtest.h>
#include "hook.hpp"

time_t fake_time(time_t *) {
    static int called = 0;
    if (called == 0) {
        ++called;
        return 0;
    } else { // When called the 2. time, fake the timeout
        return REALOAD_TIMEOUT + 1;
    }
}

bool reloadConfigCalled = false;
void mock_reloadConfig() { reloadConfigCalled = true; }

TEST(handleMessage, reloadConfig_shall_be_called_on_timeout) {
    SUBSTITUTE(&time, &fake_time);
    SUBSTITUTE(&reloadConfig, &mock_reloadConfig);
    handleMessage(Message());
    EXPECT_EQ(reloadConfigCalled, true);
}

```
Note, actually syscalls are wrapped in libc and the compiler will generate a call to the specific wrapper function.
If the compiler would emit inline assembly with the number of the syscall, then we would not be able ro replace the system calls.

### Eliminate death tests, replace `[[noreturn]]` functions
Imagine the following little command line parser function:
```c++
void parseCommandLineArgs(int argc, char** argv) {
    if (argc != 3) {
        std::exit(1);
    }
    if (std::string(argv[1]) != "apple") {
        std::exit(2);
    }
    if (std::string(argv[2]) != "banana") {
        std::exit(3);
    }
}
```

Instead of death tests we might test like this:
```c++
int exit_code = -1;
struct Exc {};
void fake_exit(int ec) {
    ::exit_code = ec;
    throw Exc{};
}

TEST_F(NoReturn, parseCommandLineArgs) {
    SUBSTITUTE_NORETURN(&std::exit, &fake_exit);

    /// argc mismatch
    exit_code = -1;
    try {
        parseCommandLineArgs(1, nullptr);
    } catch (Exc&) {
    }
    EXPECT_EQ(::exit_code, 1);

    /// first param bad
    {
        char a0[] = "prog";
        char a1[] = "asdf";
        char a2[] = "asdff";
        char* argv[3] = {a0, a1, a2};
        exit_code = -1;
        try {
            parseCommandLineArgs(3, argv);
        } catch (Exc&) {
        }
        EXPECT_EQ(::exit_code, 2);
    }

    /// second param bad
    {
        char a0[] = "prog";
        char a1[] = "apple";
        char a2[] = "asdff";
        char* argv[3] = {a0, a1, a2};
        exit_code = -1;
        try {
            parseCommandLineArgs(3, argv);
        } catch (Exc&) {
        }
        EXPECT_EQ(::exit_code, 3);
    }

    /// all params are good
    {
        char a0[] = "prog";
        char a1[] = "apple";
        char a2[] = "banana";
        char* argv[3] = {a0, a1, a2};
        exit_code = -1;
        try {
            parseCommandLineArgs(3, argv);
        } catch (Exc&) {
        }
        /// exit_code should have not changed
        EXPECT_EQ(::exit_code, -1);
    }
}
```

## How does it work?
Each and every function call expression is replaced with the following pseudo code (let's suppose, the callee is `foo`):
```c++
char* funptr = __fake_hook(&foo);
if (funptr) {
    funptr(args...);
} else {
    foo(args...);
}
```
The call to `__fake_hook` resolves in runtime if we should replace the callee to a test double or not.
If the function does return anything other than `void`:
```c++
char* funptr = __fake_hook(&foo);
auto ret = result_of(&foo);
if (funptr) {
    ret = funptr(args...);
} else {
    ret = foo(args...);
}
return ret;
```
Consider the following definition of `bar`:
```c++
int foo(int);

int bar(int p) {
    return foo(p);
}
```
The LLVM IR of `bar` after optimization looks like this:
```
define i32 @_Z3bari(i32 %p) #0 {
entry:
  %call = tail call i32 @_Z3fooi(i32 %p)
  ret i32 %call
}
```

When we enable `-fsanitize=mock` then the IR has the following form:
```
define i32 @_Z3bari(i32 %p) #0 {
entry:
  %fake_hook_result = tail call i8* @__fake_hook(i8* bitcast (i32 (i32)* @_Z3fooi to i8*))
  %0 = icmp eq i8* %fake_hook_result, null
  br i1 %0, label %else, label %then

then:                                             ; preds = %entry
  %1 = bitcast i8* %fake_hook_result to i32 (i32)*
  %subst_fun_result = tail call i32 %1(i32 %p)
  br label %cont

else:                                             ; preds = %entry
  %call = tail call i32 @_Z3fooi(i32 %p)
  br label %cont

cont:                                             ; preds = %else, %then
  %call_res.0 = phi i32 [ %subst_fun_result, %then ], [ %call, %else ]
  ret i32 %call_res.0
}
```
Actually the IR is much more verbose if optimization is not enabled, therefore
it is a good idea to use `-O2`.

### Constexpr functions
A constexpr function cannot be replaced when it is used in a compile-time expression.
However, it can be replaced whenever it is used within a runtime context:
```c++
#include "FooFixture.hpp"

namespace {

constexpr int foo(int p) { return p*p; }
int fake_foo(int p) { return p*p*p; }

constexpr int bar(int p) {
    return foo(p);
}

} // unnamed

TEST_F(FooFixture, Constexpr) {
    SUBSTITUTE(&foo, &fake_foo);

    static_assert(foo(2) == 4, "");
    EXPECT_EQ(foo(2), 8);
    int p = 2;
    EXPECT_EQ(foo(p), 8);

    static_assert(bar(2) == 4, "");
    EXPECT_EQ(bar(2), 8);
    EXPECT_EQ(bar(p), 8);
}
```

### Virtual functions
Virtual functions are problematic, because a pointer-to-member function
has a different layout in case of virtual functions than in case of regular
member functions. We have to get the address by getting the vtable from an
object and then getting the proper element of the vtable.

GCC has a construct with which we could get the address without knowing the
actually used ABI, but clang does not:
https://llvm.org/bugs/show_bug.cgi?id=22121
https://gcc.gnu.org/onlinedocs/gcc-4.9.0/gcc/Bound-member-functions.html

Note, it would be
better to have a compiler intrinsic which would simply return the adress of
the function, which is known statically during compile time.
Theoritically there is no need to provide an object.

We get the address now according to the Itanium C++ ABI.
https://mentorembedded.github.io/cxx-abi/abi.html#member-pointers
https://blog.mozilla.org/nfroyd/2014/02/20/finding-addresses-of-virtual-functions/

## How to build

Do a regular [clang build](http://clang.llvm.org/get_started.html), but instead of the original repos clone llvm, clang, libcxx from https://github.com/martong?tab=repositories .
Checkout the branch `finstrument_mock` on all repos you just have cloned.
Build the compiler.
Then, you have to obtain a googletest library (or you might use the one shipped with clang).
Once you have built the compiler then you need to build the runtime library, which is in this repository.
For exmple:
```
cmake .. -G Ninja -Dgtest_include_dir=/Users/mg/local/googletest/googletest/include -Dgtest_static_lib=/Users/mg/local/googletest/build/googlemock/gtest/libgtest.a -DCMAKE_CXX_COMPILER=/Users/mg/Work/finstrument_mock/compiler/build.debug/bin/clang++ -DCMAKE_EXPORT_COMPILE_COMMANDS=1
```
Once you have the compiler runtime and all the tests are passing then you might put the built library into one of the system library paths, so you could easily link other projects against it.
For instance, on OSX I put it under `/usr/local/lib`.

## Usage
Set up your project to use the modified compiler and link against the runtime library.
Put the header file of the runtime library into the include path of your project.
Use the `-fno-inline-functions -fsanitize=mock` switches to instrument all call expressions.

Test setup is pretty straightforward:
Use `::fake::clear()` to clear the substitutions.
Use `SUBSTITUTE(&from, &to)` to register a test double.

In case of free functions, the signature of `from` and `to` must be identical.
Unfortunately we don't assert on this currently (see future work).

In case of member functions the test double's signature must have the same signature, except the first argument.
The first argument must be a pointer to the type whose member we are replacing.

### Virtual functions
To replace virtual functions one must use the `SUBSTITUTE_VIRTUAL` macro, which requires a pointer to a (dummy) instance of the class which has the virtual function.

#### Replace virtual functions

```c++
namespace {
struct B {
    virtual int foo2(int p) { return p; }
    virtual int foo(int p) { return p; }
};
struct D : B {
    virtual int foo2(int p) override { return p + p; }
    virtual int foo(int p) override { return p + p; }
};
int B_fake_foo(B*, int p) { return p + p + p; }
int D_fake_foo(D*, int p) { return p + p + p + p; }
} // unnamed

TEST_F(FooFixture, DynamicType) {
    B* dummy = new D;
    SUBSTITUTE_VIRTUAL(&D::foo, dummy, &D_fake_foo);
    {
        B* b0 = new B;
        EXPECT_EQ(b0->foo(1), 1); // As without mock san
    }
    {
        B* b1 = new D;
        EXPECT_EQ(b1->foo(1), 4);
    }
}

TEST_F(FooFixture, DynamicType2) {
    B* dummy = new B;
    SUBSTITUTE_VIRTUAL(&B::foo, dummy, &B_fake_foo);
    {
        B* b0 = new B;
        EXPECT_EQ(b0->foo(1), 3);
    }
    {
        B* b1 = new D;
        EXPECT_EQ(b1->foo(1), 2); // As w/o mock san
    }
}
```

### [[noreturn]] functions
Use the `SUBSTITUTE_NORETURN` macro.

### Difficulties with libcxx and `always_inline` attribute
If you want to instrument those calls where the callee has the `always_inline` attribute then you have to specify `-fno-inline-functions -fsanitize=mock -fsanitize=mock_always_inline`.
Most of the getters and setters in libcxx have these attributes.
If you instrument always inline functions then make sure that they are emitted.
For example in libcxx `basic_string` template's `c_str` is always inline but there is an `extern template` declaration in the `<string>` header for `basic_string<char>`.
Therefore, your compiled object file would have an undefined reference to c_str, since the code is not emitted because of the extern template declaration.
But the already existing libcxx has an implicit template instantiation with `basic_string<char>` which does not expose the `c_str` function since that is declared always_inline.

So to make the instrumentation work either you recompile libcxx with `-fsanitize=mock` and you link against the instrumented libcxx, or you eliminate somehow the extern template declaration.
The latter is possible in the finstrument_mock branch of the libcxx repo if you define `_LIBCPP_MOCK_SAN`.
For more details please check the CMakeLists.txt in `instrument_always_inline`.

### How to access privates?
In some white box testing cases it might be a must to access privates.
More on this [here](https://github.com/martong/access_private) and [here](https://www.youtube.com/watch?v=U9Up_OfiW24).

## Performance
Our measurements show that the compilation process does not slow down noticeably.
The slow down is quite similar to other sanitizers.
The runtime performance might have a slow down of 5x - 60x.
It depends on the number of inlined functions and call expressions.
We have built the following c++ projects with `-fsanitize=mock -fno-inline-functions`:
* jsoncpp https://github.com/martong/jsoncpp/tree/finstrument_mock_test
* LLVM itself https://github.com/martong/llvm/tree/finstrument_mock_test

Currently the `-fsanitize=mock_always_inline` is tested only with the tests of this repository.

## Future work
* Enforce type checking in the header of the runtime library. http://stackoverflow.com/questions/25044869/member-function-traits http://www.boost.org/doc/libs/1_61_0/libs/type_traits/doc/html/boost_typetraits/reference/function_traits.html
* Use shadow memory (similarly as asan do in asan_rtl.cc/`AsanInitInternal()`) instead of a simple hash map in the runtime lib. That could result approximately a slowdown of 5x - 15x.
* Make more precise measurements on compilation slow down.
* Make measurements on binary size growth.

### Compile time reflection
With this instrumentation it is not possible to replace a whole type with a test double type.
We can replace only one function. This can be cumbersome, if we want to replace types.
Hopefully, in the future we might be able to write such test code:
```c++
// Don't change Entity just because of testing purposes
class Entity {
public:
  int process(int i) { if(m.try_lock()) { ... } else { ... } }
  ...
private:
  std::mutex m;
  ...
};

void testClient() {
  using EntityUnderTest =
    test::ReplaceMemberType<Entity, std::mutex, StubMutex>;
  EntityUnderTest e;

  auto& m = e.get<StubMutex>();
  m.try_lock_result = false;
  ASSERT_EQUAL(e.process(1), -1);
  m.try_lock_result = true;
  ASSERT_EQUAL(e.process(1), 1);
}

```
Of course this method has it's own disadvantages as well:
* Cannot replace a type in an already compiled translation unit.
* It works only if all type information is available i.e. if classes are header only classes.

Though, this kind of compile time reflection would be a nice complement of the instrumentation presented in this repository.

## Outlook
You can hear more about non-intrusive testing [here](https://www.youtube.com/watch?v=U9Up_OfiW24).

## Acknowledgement
Thanks to Imre Szekeres, Gábor Horváth, Péter Bolla, Máté Csákvári, Zoltán Porkoláb, Zsolt Parragi for all the discussions we had.
