#include "FooFixture.hpp"

struct DynamicType : FooFixture {};

namespace Basic {
enum class Values : int { B, D, FB, FD };
struct B {
    virtual Values foo(int p) { return Values::B; }
};
struct D : B {
    virtual Values foo(int p) override { return Values::D; }
};
Values B_fake_foo(B*, int p) { return Values::FB; }
Values D_fake_foo(D*, int p) { return Values::FD; }
}

TEST_F(DynamicType, Basic_D) {
    using Basic::B;
    using Basic::D;
    using Basic::D_fake_foo;
    using Basic::Values;

    B* dummy = new D;
    SUBSTITUTE_VIRTUAL(&D::foo, dummy, &D_fake_foo);
    {
        B* b0 = new B;
        EXPECT_EQ(b0->foo(1), Values::B);
    }
    {
        B* b1 = new D;
        EXPECT_EQ(b1->foo(1), Values::FD);
    }
}

TEST_F(DynamicType, Basic_B) {
    using Basic::B;
    using Basic::D;
    using Basic::B_fake_foo;
    using Basic::Values;

    B* dummy = new B;
    SUBSTITUTE_VIRTUAL(&B::foo, dummy, &B_fake_foo);
    {
        B* b0 = new B;
        EXPECT_EQ(b0->foo(1), Values::FB);
    }
    {
        B* b1 = new D;
        EXPECT_EQ(b1->foo(1), Values::D);
    }
}

namespace InheritanceChain {
enum class Values : int { B, C, D, FB, FC, FD };
struct B {
    virtual Values foo(int p) { return Values::B; }
};
struct C : B {
    virtual Values foo(int p) { return Values::C; }
};
struct D : C {
    virtual Values foo(int p) override { return Values::D; }
};
Values B_fake_foo(B*, int p) { return Values::FB; }
Values C_fake_foo(C*, int p) { return Values::FC; }
Values D_fake_foo(D*, int p) { return Values::FD; }
}

TEST_F(DynamicType, InheritanceChain_D) {
    using InheritanceChain::Values;
    using InheritanceChain::B;
    using InheritanceChain::C;
    using InheritanceChain::D;
    using InheritanceChain::D_fake_foo;

    B* dummy = new D;
    SUBSTITUTE_VIRTUAL(&D::foo, dummy, &D_fake_foo);
    {
        B* b0 = new B;
        EXPECT_EQ(b0->foo(1), Values::B);
    }
    {
        C* b0 = new C;
        EXPECT_EQ(b0->foo(1), Values::C);
    }
    {
        B* b1 = new D;
        EXPECT_EQ(b1->foo(1), Values::FD);
    }
}

TEST_F(DynamicType, InheritanceChain_C) {
    using InheritanceChain::Values;
    using InheritanceChain::B;
    using InheritanceChain::C;
    using InheritanceChain::D;
    using InheritanceChain::C_fake_foo;

    B* dummy = new C;
    SUBSTITUTE_VIRTUAL(&C::foo, dummy, &C_fake_foo);
    {
        B* b0 = new B;
        EXPECT_EQ(b0->foo(1), Values::B);
    }
    {
        C* b0 = new C;
        EXPECT_EQ(b0->foo(1), Values::FC);
    }
    {
        B* b1 = new D;
        EXPECT_EQ(b1->foo(1), Values::D);
    }
}

TEST_F(DynamicType, InheritanceChain_B) {
    using InheritanceChain::Values;
    using InheritanceChain::B;
    using InheritanceChain::C;
    using InheritanceChain::D;
    using InheritanceChain::B_fake_foo;

    B* dummy = new B;
    SUBSTITUTE_VIRTUAL(&B::foo, dummy, &B_fake_foo);
    {
        B* b0 = new B;
        EXPECT_EQ(b0->foo(1), Values::FB);
    }
    {
        C* b0 = new C;
        EXPECT_EQ(b0->foo(1), Values::C);
    }
    {
        B* b1 = new D;
        EXPECT_EQ(b1->foo(1), Values::D);
    }
}

namespace MultipleInheritance {
enum class Values : int { B, C, D, FB, FC, FD };
struct B {
    virtual Values foo(int p) { return Values::B; }
};
struct C {
    virtual Values foo(int p) { return Values::C; }
};
struct D : B, C {
    virtual Values foo(int p) override { return Values::D; }
};
Values B_fake_foo(B*, int p) { return Values::FB; }
Values C_fake_foo(C*, int p) { return Values::FC; }
Values D_fake_foo(D*, int p) { return Values::FD; }
}


TEST_F(DynamicType, MultipleInheritance_D) {
    using MultipleInheritance::Values;
    using MultipleInheritance::B;
    using MultipleInheritance::C;
    using MultipleInheritance::D;
    using MultipleInheritance::D_fake_foo;

    B* dummy = new D;
    SUBSTITUTE_VIRTUAL(&D::foo, dummy, &D_fake_foo);
    {
        B* b0 = new B;
        EXPECT_EQ(b0->foo(1), Values::B);
    }
    {
        C* b0 = new C;
        EXPECT_EQ(b0->foo(1), Values::C);
    }
    {
        B* b1 = new D;
        EXPECT_EQ(b1->foo(1), Values::FD);
    }
}

TEST_F(DynamicType, MultipleInheritance_C) {
    using MultipleInheritance::Values;
    using MultipleInheritance::B;
    using MultipleInheritance::C;
    using MultipleInheritance::D;
    using MultipleInheritance::C_fake_foo;

    C* dummy = new C;
    SUBSTITUTE_VIRTUAL(&C::foo, dummy, &C_fake_foo);
    {
        B* b0 = new B;
        EXPECT_EQ(b0->foo(1), Values::B);
    }
    {
        C* b0 = new C;
        EXPECT_EQ(b0->foo(1), Values::FC);
    }
    {
        B* b1 = new D;
        EXPECT_EQ(b1->foo(1), Values::D);
    }
}

TEST_F(DynamicType, MultipleInheritance_B) {
    using MultipleInheritance::Values;
    using MultipleInheritance::B;
    using MultipleInheritance::C;
    using MultipleInheritance::D;
    using MultipleInheritance::B_fake_foo;

    B* dummy = new B;
    SUBSTITUTE_VIRTUAL(&B::foo, dummy, &B_fake_foo);
    {
        B* b0 = new B;
        EXPECT_EQ(b0->foo(1), Values::FB);
    }
    {
        C* b0 = new C;
        EXPECT_EQ(b0->foo(1), Values::C);
    }
    {
        B* b1 = new D;
        EXPECT_EQ(b1->foo(1), Values::D);
    }
}

// TODO virtual inheritance
