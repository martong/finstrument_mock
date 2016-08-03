#include "Entity.hpp"

int Entity::process(int i) {
    if (m.try_lock()) {
        auto result = std::accumulate(v.begin(), v.end(), i);
        m.unlock();
        return result;
    } else {
        return -1;
    }
    return 0;
}
