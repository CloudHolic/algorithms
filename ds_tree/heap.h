#pragma once

#include <vector>

using namespace std;

class heap
{
public:
    heap(int size);

    void insert(const int value);
    void pop();
    int peak() const;
    void clear();

private:
    vector<int> nodes_;
    int size_;
};