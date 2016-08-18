#include "FooFixture.hpp"

namespace {
struct B {
    virtual int foo(int p) { return p; }
};
struct D : B {
    virtual int foo(int p) override { return p + p; }
};
int B_fake_foo(B*, int p) { return p + p + p; }
int D_fake_foo(D*, int p) { return p + p + p + p; }
} // unnamed

TEST_F(FooFixture, DynamicType) {
    SUBSTITUTE(&D::foo, &D_fake_foo);
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
    SUBSTITUTE(&B::foo, &B_fake_foo);
    {
        B* b0 = new B;
        EXPECT_EQ(b0->foo(1), 3);
    }
    {
        B* b1 = new D;
        EXPECT_EQ(b1->foo(1), 2); // As w/o mock san
    }
}
