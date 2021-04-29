#include "segment_tree.h"

segment_tree::segment_tree(int arr[], int size)
{
    nodes_ = vector<int*>();
    build(arr, size);
}

segment_tree::~segment_tree()
{
    clear();
}

void segment_tree::build(int arr[], int size)
{
    size_ = size;
    nodes_.assign(2 * size, nullptr);

    for(int i = 0; i < size; i++)
        nodes_[size + i] = new int(arr[size]);

    for(int i = size - 1; i > 0; i--)
        nodes_[i] = new int(*nodes_[i << 1] + *nodes_[i << 1 | 1]);
}

void segment_tree::update(int index, int value)
{
    delete nodes_[size_ + index];
    nodes_[size_ + index] = new int(value);
    
    for(int i = size_ + index; i > 1; i >>= 1)
    {
        delete nodes_[i >> 1];
        nodes_[i >> 1] = new int(*nodes_[i] + *nodes_[i ^ 1]);
    }
}

int segment_tree::query(int left, int right)
{
    int result = 0;

    for(left += size_, right += size_; left < right; left >>= 1, right >>= 1)
    {
        if(left & 1)
            result += *nodes_[left++];
        if(right & 1)
            result += *nodes_[--right];
    }

    return result;
}

void segment_tree::clear()
{
    for(int i = 0; i < size_ * 2; i++)
        if(nodes_[i] != nullptr)
            delete nodes_[i];

    nodes_.clear();
    size_ = 0;
}