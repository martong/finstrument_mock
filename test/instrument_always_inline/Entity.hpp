#include <mutex>
#include <vector>

class Entity {
public:
    int process(int i) const;
    void add(int i);

private:
    std::vector<int> v;
    mutable std::mutex m;
};
