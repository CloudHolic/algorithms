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
    if(current_size_ >= max_size_)
        throw std::range_error("BST is already full.");

    int index = 1;

    while(nodes_[index] != nullptr)
    {     
        if(*nodes_[index] < value)
            index *= 2;
        else if(*nodes_[index] > value)
            index = index * 2 + 1;            
        else // if(*nodes_[index] == value)
            throw std::invalid_argument("No duplicated value allowed in BST.");

        if(index > max_size_)
            throw std:: invalid_argument("BST is already full.");
    }

    nodes_[index] = new int(value);
    current_size_++;
}

void bst::remove(const int value)
{
    if(current_size_ <= 0)
        throw std::range_error("BST is already empty.");
}

int bst::find_index(const int value) const
{
    for(int index = 1; nodes_[index] != nullptr; )
    {
        if(*nodes_[index] < value)
            index *= 2;
        else if(*nodes_[index] > value)
            index = index * 2 + 1;
        else // if(*nodes_[index] == value)
            return index;

        if(index > max_size_)
            break;
    }

    return -1;
}

void bst::clear()
{
    for(int i = 0; i < current_size_; i++)
        if(nodes_[i] != nullptr)
            delete nodes_[i];

    current_size_ = 0;
}