#include "FooFixture.hpp"

template <typename T>
struct TemplateS {
    int foo(int p) { return bar(p); }
    int bar(int p) { return p; }
};
int fake_bar_mem_fun(TemplateS<int>* self, int p) { return p * 3; }
TEST_F(FooFixture, ClassT) {
    SUBSTITUTE(&TemplateS<int>::bar, &fake_bar_mem_fun);
    TemplateS<int> t;
    auto res = t.foo(13);
    EXPECT_EQ(res, 39);
}

template <typename T>
T FunTemp(T t) {
    return t;
}
int fake_FunTemp(int p) { return p * 3; }
TEST_F(FooFixture, FunT) {
    SUBSTITUTE(&FunTemp<int>, &fake_FunTemp);
    int p = 13;
    auto res = FunTemp(p);
    EXPECT_EQ(res, 39);
}

struct Class {
    template <typename T>
    T FunTemp(T t) {
        return t;
    }
};
int fake_Class_mf(Class*, int p) { return p * 3; }
TEST_F(FooFixture, ClassFunT) {
    SUBSTITUTE(&Class::FunTemp<int>, &fake_Class_mf);
    Class c;
    int p = 13;
    auto res = c.FunTemp(p);
    EXPECT_EQ(res, 39);
}

template <typename T>
struct TemplateS2 {
    template <typename U>
    U fooT(U p) {
        return barT(p);
    }
    template <typename U>
    U barT(U p) {
        return p;
    }
};
TEST_F(FooFixture, ClassTFunT) {
    SUBSTITUTE(&TemplateS2<int>::barT<int>, &fake_bar_mem_fun);
    TemplateS2<int> t;
    int p = 13;
    auto res = t.barT(p);
    EXPECT_EQ(res, 39);
    res = t.fooT(p);
    EXPECT_EQ(res, 39);
}
