#include <mutex>
#include <numeric>
#include <vector>

class Entity {
public:
    int process(int i) const {
        std::unique_lock<std::mutex> lock{m, std::try_to_lock};
        if (lock.owns_lock()) {
            auto result = std::accumulate(v.begin(), v.end(), i);
            return result;
        } else {
            return -1;
        }
        return 0;
    }
    void add(int i) {
        std::lock_guard<std::mutex> lock{m};
        v.push_back(i);
    }

private:
    std::vector<int> v;
    mutable std::mutex m;
};
