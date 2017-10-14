#include "FooFixture.hpp"

struct FunctionId : FooFixture {};

namespace {
namespace InheritanceChain {
enum class Values : int { B, C, D };
struct B {
    Values foo(int p) { return Values::B; }
};
struct C : B {
    Values foo(int p) { return Values::C; }
};
struct D : C {
    Values foo(int p) { return Values::D; }
};
}
}

TEST_F(FunctionId, InheritanceChain) {
    using InheritanceChain::B;
    using InheritanceChain::C;
    using InheritanceChain::D;

    {
        auto p1 = fake::address(&B::foo);
        auto p2 = (const char*) __function_id B::foo;
        EXPECT_EQ(p1, p2);
    }
    {
        auto p1 = fake::address(&C::foo);
        auto p2 = (const char*) __function_id C::foo;
        EXPECT_EQ(p1, p2);
    }
    {
        auto p1 = fake::address(&D::foo);
        auto p2 = (const char*) __function_id D::foo;
        EXPECT_EQ(p1, p2);
    }
}

namespace {
namespace InheritanceChainWithVirtuals {
enum class Values : int { B, C, D };
struct B {
    virtual Values foo(int p) { return Values::B; }
};
struct C : B {
    virtual Values foo(int p) override { return Values::C; }
};
struct D : C {
    virtual Values foo(int p) override { return Values::D; }
};
}
}

TEST_F(FunctionId, InheritanceChainWithVirtuals) {
    using InheritanceChainWithVirtuals::B;
    using InheritanceChainWithVirtuals::C;
    using InheritanceChainWithVirtuals::D;

    {
        B* b0 = new B;
        auto p1 = (const char*) fake::address_of_virtual_fun(b0, &B::foo);
        auto p2 = (const char*) __function_id B::foo;
        EXPECT_EQ(p1, p2);
    }
    {
        B* b0 = new C;
        auto p1 = (const char*) fake::address_of_virtual_fun(b0, &C::foo);
        auto p2 = (const char*) __function_id C::foo;
        EXPECT_EQ(p1, p2);
    }
    {
        C* b0 = new C;
        auto p1 = (const char*) fake::address_of_virtual_fun(b0, &C::foo);
        auto p2 = (const char*) __function_id C::foo;
        EXPECT_EQ(p1, p2);
    }
    {
        B* b0 = new D;
        auto p1 = (const char*) fake::address_of_virtual_fun(b0, &D::foo);
        auto p2 = (const char*) __function_id D::foo;
        EXPECT_EQ(p1, p2);
    }
}

namespace {
namespace MultipleInheritance {
enum class Values : int { B, C, D };
struct B {
    virtual Values foo(int p) { return Values::B; }
};
struct C {
    virtual Values foo(int p) { return Values::C; }
};
struct D : B, C {
    virtual Values foo(int p) override { return Values::D; }
};
}
}

TEST_F(FunctionId, MultipleInheritance) {
    using MultipleInheritance::B;
    using MultipleInheritance::C;
    using MultipleInheritance::D;
    {
        B* b0 = new B;
        auto p1 = (const char*) fake::address_of_virtual_fun(b0, &B::foo);
        auto p2 = (const char*) __function_id B::foo;
        EXPECT_EQ(p1, p2);
    }
    {
        C* b0 = new C;
        auto p1 = (const char*) fake::address_of_virtual_fun(b0, &C::foo);
        auto p2 = (const char*) __function_id C::foo;
        EXPECT_EQ(p1, p2);
    }
    {
        B* b0 = new D;
        auto p1 = (const char*) fake::address_of_virtual_fun(b0, &D::foo);
        auto p2 = (const char*) __function_id D::foo;
        EXPECT_EQ(p1, p2);
    }
}


namespace {
namespace Dreaded {

enum class Values : int { B, C1, C2, D, Bbar };
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
}
}

TEST_F(FunctionId, Dreaded) {
    using Dreaded::B;
    using Dreaded::C1;
    using Dreaded::C2;
    using Dreaded::D;
    {
        B* b0 = new B;
        auto p1 = (const char*) fake::address_of_virtual_fun(b0, &B::foo);
        auto p2 = (const char*) __function_id B::foo;
        EXPECT_EQ(p1, p2);
    }
    {
        B* b0 = new C1;
        auto p1 = (const char*) fake::address_of_virtual_fun(b0, &C1::foo);
        auto p2 = (const char*) __function_id C1::foo;
        EXPECT_EQ(p1, p2);
    }
    {
        C1* b0 = new C1;
        auto p1 = (const char*) fake::address_of_virtual_fun(b0, &C1::foo);
        auto p2 = (const char*) __function_id C1::foo;
        EXPECT_EQ(p1, p2);
    }
    {
        B* b0 = new C2;
        auto p1 = (const char*) fake::address_of_virtual_fun(b0, &C2::foo);
        auto p2 = (const char*) __function_id C2::foo;
        EXPECT_EQ(p1, p2);
    }
    {
        C2* b0 = new C2;
        auto p1 = (const char*) fake::address_of_virtual_fun(b0, &C2::foo);
        auto p2 = (const char*) __function_id C2::foo;
        EXPECT_EQ(p1, p2);
    }
    {
        B* b0 = new D;
        auto p1 = (const char*) fake::address_of_virtual_fun(b0, &D::foo);
        auto p2 = (const char*) __function_id D::foo;
        EXPECT_EQ(p1, p2);
    }
}
