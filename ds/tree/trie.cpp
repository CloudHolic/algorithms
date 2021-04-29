#include "trie.h"

trie::trie()
{
    root_ = new node();
}

trie::~trie()
{
    clear();
}

void trie::insert(string value)
{
    if(root_ == nullptr)
        root_ = new node();

    node* temp = root_;
    for(int i = 0; i < value.length(); i++)
    {
        int index = value[i] - 'a';
        if(temp->children[index] == nullptr)
        {
            temp->children[index] = new node();
            temp->children[index]->parent = temp;
        }

        temp = temp->children[index];
    }

    temp->is_end = true;
}

void trie::insert(string values[], int size)
{
    for(int i = 0; i < size; i++)
        insert(values[i]);
}

bool trie::exist(string value)
{
    if(root_ == nullptr)
        return false;

    node* temp = root_;
    for(int i = 0; i < value.length(); i++)
    {
        int index = value[i] - 'a';
        if(temp->children[i] == nullptr)
            return false;
        
        temp = temp->children[index];
    }

    return temp != nullptr && temp->is_end;
}

void trie::clear()
{
    internal_clear(root_, 0);
}

void trie::internal_clear(node* start, int index)
{
    for(int i = 0; i < ALPHABET_SIZE; i++)    
        if(start->children[i] != nullptr)
            internal_clear(start->children[i], i);

    if(start->parent != nullptr)
        start->parent->children[index] = nullptr;

    delete start;
}

trie::node::node()
{
    parent = nullptr;
    for(int i = 0; i < ALPHABET_SIZE; i++)
        children[i] = nullptr;

    is_end = false;
}