#include "VirtX.hpp"
#include "called.hpp"
#include "address.hpp"
#include "hook.hpp"

void VirtX::foo(int) {
    ::ftest::called.insert(address(&VirtX::foo));
}
