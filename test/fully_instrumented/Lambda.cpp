#include "FooFixture.hpp"

struct Lambda : FooFixture {};

struct Struct {
    int foo() {
        auto lam = [](){ return 1; };
        return lam();
    }
};

//TEST_F(Lambda, First){
    // error: 'foo' is not a class, namespace, or enumeration
    //Struct::foo::lam
//}
