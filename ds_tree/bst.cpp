#include <stdexcept>
#include "bst.h"

bst::bst(int size)
{
    nodes_ = vector<int*>(size + 1, nullptr);
    max_size_ = size + 1;
    current_size_ = 0;
}

bst::~bst()
{
    clear();
}

int bst::operator[](const int index)
{
    return *nodes_[index];
}

void bst::insert(const int value)
{
    if (current_size_ >= max_size_)
        throw std::range_error("BST is already full.");

    int index = 1;

    while (nodes_[index] != nullptr)
    {     
        if (*nodes_[index] < value)
            index *= 2;
        else if (*nodes_[index] > value)
            index = index * 2 + 1;
        else // if (*nodes_[index] == value)
            throw std::invalid_argument("No duplicated value allowed in BST.");

        if (index > max_size_)
            throw std:: range_error("BST is already full.");
    }

    nodes_[index] = new int(value);
    current_size_++;
}

void bst::remove(const int value)
{
    if (current_size_ <= 0)
        throw std::range_error("BST is already empty.");

    int index = find_index(value);
    if (index < 1)
        throw std::range_error("No such value in BST");

    if (nodes_[index * 2] == nullptr && nodes_[index * 2 + 1] == nullptr)
        move_subtree(index, 0);
    else if (nodes_[index * 2] == nullptr)    
        move_subtree(index, index * 2 + 1);    
    else if (nodes_[index * 2 + 1] == nullptr)    
        move_subtree(index, index * 2);    
    else
    {
        delete nodes_[index];
        int child = index * 2 + 1;
        while(child <= max_size_ && nodes_[child] != nullptr)
            child *= 2;
        
        if(child > max_size_ || nodes_[child] == nullptr)
            child /= 2;

        nodes_[index] = nodes_[child];
        nodes_[child] = nullptr;
    }
}

int bst::find_index(const int value) const
{
    for (int index = 1; nodes_[index] != nullptr; )
    {
        if (*nodes_[index] < value)
            index *= 2;
        else if (*nodes_[index] > value)
            index = index * 2 + 1;
        else // if (*nodes_[index] == value)
            return index;

        if (index > max_size_)
            break;
    }

    return -1;
}

void bst::clear()
{
    for (int i = 0; i < current_size_; i++)
        if (nodes_[i] != nullptr)
            delete nodes_[i];

    current_size_ = 0;
}

void bst::move_subtree(int parent, int child)
{
    delete nodes_[parent];
    if(child == 0)
        return;
    
    nodes_[parent] = nodes_[child];
    parent *= 2;
    child *= 2;

    while (child < max_size_ && (nodes_[child] != nullptr || nodes_[child + 1] != nullptr))
    {
        if (nodes_[child] != nullptr)
            nodes_[parent] = nodes_[child];
        if (nodes_[child + 1] != nullptr)
            nodes_[parent + 1] = nodes_[child + 1];

        parent *= 2;
        child *= 2;
    }
}