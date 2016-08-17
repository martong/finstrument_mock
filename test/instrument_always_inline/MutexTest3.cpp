#include "FooFixture.hpp"

#include "Entity2.hpp"

namespace {

bool owns_lock_result;
using Lock = std::unique_lock<std::mutex>;
bool fake_owns_lock(Lock*) { return owns_lock_result; }

} // unnamed

TEST_F(FooFixture, Mutex3) {
    SUBSTITUTE(&Lock::owns_lock, &fake_owns_lock);
    Entity e;
    owns_lock_result = false;
    EXPECT_EQ(e.process(1), -1);
    owns_lock_result = true;
    EXPECT_EQ(e.process(1), 1);
}
