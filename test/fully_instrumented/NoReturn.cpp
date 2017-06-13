#include "hook.hpp"
#include "FooFixture.hpp"
#include <cstdlib>
#include <string>

struct NoReturn : FooFixture {};

namespace {
[[noreturn]] void foo(int) { exit(0); };
void fake_foo(int p) {
    EXPECT_EQ(p, 13);
    ::ftest::called.insert(address(&fake_foo));
}
void bar() { foo(13); };
} // unnamed
TEST_F(NoReturn, foo) {
    SUBSTITUTE(&foo, &fake_foo);
    bar();
    EXPECT_EQ(::ftest::called.count(address(fake_foo)), 1);
}

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

int exit_code = -1;
struct Exc {};
void fake_exit(int ec) {
    ::exit_code = ec;
    throw Exc{};
}

TEST_F(NoReturn, parseCommandLineArgs) {
    SUBSTITUTE(&std::exit, &fake_exit);

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
