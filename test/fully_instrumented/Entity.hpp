#include <mutex>
#include <numeric>
#include <vector>

class Entity {
public:
    int process(int i) {
        if (m.try_lock()) {
            auto result = std::accumulate(v.begin(), v.end(), i);
            m.unlock();
            return result;
        }
        return -1;
    }
    void add(int i) { v.push_back(i); }

private:
    std::vector<int> v;
    std::mutex m;
};
