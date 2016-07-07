#include <gtest/gtest.h>
#include "hook.hpp"

#include "foo.hpp"
#include "called.hpp"

void fake_foo() {
    test::called.insert(reinterpret_cast<char *>(fake_foo));
    printf("fake_foo called\n");
}

// bar calls foo and foo2,
// we want to fake foo, but not foo2
void bar();

TEST(Foo, Z3foov) {
    ::fake::subs.insert(
        {reinterpret_cast<char *>(foo), reinterpret_cast<char *>(fake_foo)});
    bar();

    EXPECT_EQ(test::called.count(reinterpret_cast<char *>(foo)), 0);
    EXPECT_EQ(test::called.count(reinterpret_cast<char *>(fake_foo)), 1);
    EXPECT_EQ(test::called.count(reinterpret_cast<char *>(foo2)), 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

