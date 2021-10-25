#pragma once

#include <string>

#define ALPHABET_SIZE 26

using namespace std;

class trie
{
public:
    // Constructor
    trie();

    // Destructor
    ~trie();

    // Other methods
    void insert(string value);
    void insert(string values[], int size);
    bool exist(string value);
    void clear();

private:
    struct node
    {
        node* parent;
        node* children[ALPHABET_SIZE];
        bool is_end;

        node();
    };

    node* root_;

    // Private method
    void internal_clear(node* start, int index);
};