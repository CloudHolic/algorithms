#pragma once

#include <vector>

using namespace std;

class segment_tree
{
public:
    // Constructor
    segment_tree(int arr[], int size);

    // Destructor
    ~segment_tree();

    // Other methods
    void build(int arr[], int size);
    void update(int index, int value);
    int query(int left, int right);
    void clear();
    
private:
    vector<int*> nodes_;
    int size_;
};