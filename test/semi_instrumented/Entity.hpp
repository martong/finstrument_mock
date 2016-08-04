#pragma once

#include <mutex>
#include <numeric>
#include <vector>

class Entity {
public:
    int process(int i);
    void add(int i) { v.push_back(i); }

private:
    std::vector<int> v;
    std::mutex m;
};
