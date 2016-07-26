#include <unordered_map>

namespace fake {
extern std::unordered_map<char *, char *> subs;
}

template <typename T>
char *address(T t) {
    // This is the only way to get the absolute address of a member function
    // http://stackoverflow.com/questions/8121320/get-memory-address-of-member-function
    return (char *&)(t);
}
