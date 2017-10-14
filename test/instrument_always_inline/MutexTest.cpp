#include "FooFixture.hpp"

#include "Entity.hpp"

struct AlwaysInline : FooFixture {};

namespace {

bool owns_lock_result;
using Lock = std::unique_lock<std::mutex>;
bool fake_owns_lock(Lock*) { return owns_lock_result; }

} // unnamed

TEST_F(AlwaysInline, MutexTest) {
    SUBSTITUTE(Lock::owns_lock, fake_owns_lock);
    Entity e;
    owns_lock_result = false;
    EXPECT_EQ(e.process(1), -1);
    owns_lock_result = true;
    EXPECT_EQ(e.process(1), 1);
}
