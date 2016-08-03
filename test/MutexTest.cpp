#include "FooFixture.hpp"

#include "Entity.hpp"

bool try_lock_result;
bool fake_mutex_try_lock(std::mutex* self) { return try_lock_result; }

TEST_F(FooFixture, Mutex) {
    ::fake::subs.insert(
        {address(&std::mutex::try_lock), address(fake_mutex_try_lock)});
    Entity e;
    try_lock_result = false;
    EXPECT_EQ(e.process(1), -1);
    try_lock_result = true;
    EXPECT_EQ(e.process(1), 1);
}
