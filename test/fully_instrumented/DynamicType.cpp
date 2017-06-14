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

namespace Dreaded {

enum class Values : int { B, C1, C2, D, FB, FC1, FC2, FD, Bbar };
class B {
public:
    virtual Values foo() { return Values::B; }
    virtual Values bar() { return Values::Bbar; }
};
class C1 : public virtual B {
public:
    virtual Values foo() { return bar(); }
};

class C2 : public virtual B {
public:
    virtual Values bar() { return Values::C2; }
};

class D : public C1, public C2 {};
Values C2_fake_foo(C2*) { return Values::FC2; }
}

// Inspired from isocpp cpp FAQ:
// https://isocpp.org/wiki/faq/multiple-inheritance#mi-delegate-to-sister
TEST_F(DynamicType, Dreaded) {
    using Dreaded::B;
    using Dreaded::C1;
    using Dreaded::C2;
    using Dreaded::D;
    using Dreaded::Values;
    using Dreaded::C2_fake_foo;

    // normally would call C2::bar()
    {
        D* p1 = new D();
        EXPECT_EQ(p1->foo(), Values::C2);
    }

    {
        C2* dummy = new C2();
        SUBSTITUTE_VIRTUAL(&C2::bar, dummy, C2_fake_foo);

        D* p1 = new D();
        EXPECT_EQ(p1->foo(), Values::FC2);
    }
}

namespace Dreaded2 {

enum class Values : int { B, C1, C2, D, FB, FC1, FC2, FD, Bbar };
class B {
public:
    virtual Values foo() { return Values::B; }
    virtual Values bar() { return Values::Bbar; }
};
class C1 : public virtual B {
public:
    virtual Values foo() { return Values::C1; }
};

class C2 : public virtual B {
public:
    virtual Values bar() { return Values::C2; }
};

class D : public C1, public C2 {
public:
    virtual Values foo() { return Values::D; }
    virtual Values bar() { return Values::D; }
};

Values C1_fake_foo(C1*) { return Values::FC1; }
Values C2_fake_foo(C2*) { return Values::FC2; }

} // namespace Dreaded2


// Inspired from isocpp cpp FAQ:
// https://isocpp.org/wiki/faq/multiple-inheritance#mi-delegate-to-sister
TEST_F(DynamicType, Dreaded2) {
    using Dreaded2::B;
    using Dreaded2::C1;
    using Dreaded2::C2;
    using Dreaded2::D;
    using Dreaded2::Values;
    using Dreaded2::C1_fake_foo;
    using Dreaded2::C2_fake_foo;

    {
        D* p1 = new D();
        EXPECT_EQ(p1->foo(), Values::D);
        EXPECT_EQ(p1->bar(), Values::D);
    }

    {
        C1* dummy = new C1();
        SUBSTITUTE_VIRTUAL(&C1::foo, dummy, C1_fake_foo);

        B* p1 = new C1();
        EXPECT_EQ(p1->foo(), Values::FC1);
    }

    {
        C2* dummy = new C2();
        SUBSTITUTE_VIRTUAL(&C2::bar, dummy, C2_fake_foo);

        B* p1 = new C2();
        EXPECT_EQ(p1->bar(), Values::FC2);
    }

    {
        B* p1 = new D();
        EXPECT_EQ(p1->bar(), Values::D);
    }
}
