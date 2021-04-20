#pragma once

#include <vector>

using namespace std;

class heap
{
public:
    // Constructor
    heap(int size);

    // Destructor
    ~heap();

    void insert(const int value);
    void pop();
    int peak() const;
    void clear();

private:
    vector<int*> nodes_;
    int max_size_, current_size_;
};