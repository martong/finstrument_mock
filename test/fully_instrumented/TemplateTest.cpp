#include "FooFixture.hpp"

#include "Template.hpp"

int fake_bar_mem_fun(TemplateS<int>* self, int p) {
  return p * 3;
}

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
int fake_FunTemp(int p) {
  return p * 3;
}
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
int fake_Class_mf(Class*, int p) {
  return p * 3;
}
TEST_F(FooFixture, ClassFunT) {
  SUBSTITUTE(&Class::FunTemp<int>, &fake_Class_mf);
  Class c;
  int p = 13;
  auto res = c.FunTemp(p);
  EXPECT_EQ(res, 39);
}

TEST_F(FooFixture, ClassTFunT) {
  SUBSTITUTE(&TemplateS<int>::barT<int>, &fake_bar_mem_fun);
  TemplateS<int> t;
  int p = 13;
  auto res = t.barT(p);
  EXPECT_EQ(res, 39);
  res = t.fooT(p);
  EXPECT_EQ(res, 39);
}
