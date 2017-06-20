#include <hook.h>
#include "increment.h"

int main(int argc, char** argv) {
    /*_substitute_function((const char*)&increment, (const char*)&increment);*/
    _substitute_function((const char*)&increment, (const char*)&fake_increment);
    return Main(argc, argv);
}
