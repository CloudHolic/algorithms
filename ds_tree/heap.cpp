#include <stdexcept>
#include "heap.h"

heap::heap(int size)
{
    nodes_ = vector<int*>(size + 1, nullptr);
    max_size_ = size + 1;
    current_size_ = 0;
}

heap::~heap()
{
    clear();
}

void heap::insert(const int value)
{
    if(current_size_ >= max_size_)
        throw std::range_error("Heap is already full.");

    int index = ++current_size_;

    while(index != 1 && *nodes_[index / 2] < value)
    {
        nodes_[index] = nodes_[index / 2];
        index /= 2;
    }

    nodes_[index] = new int(value);
}

void heap::pop()
{
    if(current_size_ <= 0)
        throw std::range_error("Heap is already empty.");

    int parent_node = 1, child_node = 2;
    int* temp;
    delete nodes_[1];
    temp = nodes_[current_size_--];

    while(child_node <= current_size_)
    {
        if(child_node < current_size_ && *nodes_[child_node] < *nodes_[child_node + 1])
            child_node++;

        if(*temp >= *nodes_[child_node])
            break;

        nodes_[parent_node] = nodes_[child_node];
        parent_node = child_node;
        child_node *= 2;
    }

    nodes_[parent_node] = temp;
}

int heap::peak() const
{
    return *(nodes_[1]);
}

void heap::clear()
{
    for(int i = 0; i < current_size_; i++)
        delete nodes_[i];
    
    current_size_ = 0;
}