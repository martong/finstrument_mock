#include <unordered_map>

namespace fake {

void clear();
void insert(std::pair<char*, char*>);

template <typename T>
char *address(T t) {
    // This is the only way to get the absolute address of a member function
    // http://stackoverflow.com/questions/8121320/get-memory-address-of-member-function
    return (char *&)(t);
}


}

#define SUBSTITUTE(src, dst)                                              \
    do {                                                                  \
        ::fake::insert({::fake::address((src)), ::fake::address((dst))}); \
    } while (0)
