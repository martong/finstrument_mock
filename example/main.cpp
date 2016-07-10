#include <gtest/gtest.h>
#include "hook.hpp"

#include "foo.hpp"
#include "called.hpp"

void fake_fooRvoidPvoid() {
    test::called.insert(reinterpret_cast<char *>(fake_fooRvoidPvoid));
}

/// bar calls foo and foo_2,
/// we want to fake foo, but not foo_2
void bar();

/// signature: void()
TEST(Foo, fooRvoidPvoid) {
    ::fake::subs.insert({reinterpret_cast<char *>(fooRvoidPvoid),
                         reinterpret_cast<char *>(fake_fooRvoidPvoid)});
    bar();

    EXPECT_EQ(test::called.count(reinterpret_cast<char *>(fooRvoidPvoid)), 0);
    EXPECT_EQ(test::called.count(reinterpret_cast<char *>(fake_fooRvoidPvoid)),
              1);
    EXPECT_EQ(test::called.count(reinterpret_cast<char *>(fooRvoidPvoid_2)), 1);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

