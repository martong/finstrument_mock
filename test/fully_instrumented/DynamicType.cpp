#include "FooFixture.hpp"

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
