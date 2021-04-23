#pragma once

#include <vector>

using namespace std;

class bst
{
public:
    // Constructor
    bst(int size);

    // Destructor
    ~bst();

    // Operator overloading
    int operator[](const int index);

    void insert(const int value);
    void remove(const int value);
    int find_index(const int value) const;

    void clear();

private:
    vector<int*> nodes_;
    int max_size_, current_size_;

    void move_subtree(int parent, int child);
};