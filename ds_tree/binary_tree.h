#pragma once

#include <vector>

using namespace std;

template <typename T>
class tree_node
{
    friend class binary_tree;

public:
    tree_node(T value);        
    
private:
    T data;
    tree_node* left_child;
    tree_node* right_child;
};

template <typename T>
class binary_tree
{
public:
    // Constructor
    binary_tree(T root_value);

    // Destructor
    ~binary_tree();

    tree_node<T>* get_root();
    tree_node<T>* get_child(tree_node<T>* parent_node, bool is_left);
    void insert(tree_node<T>* parent_node, T value, bool is_left);

    vector<T> preorder() const;
    vector<T> inorder() const;
    vector<T> postorder() const;

private:
    tree_node<T>* root_;
    int size_;

    void clear(tree_node<T>* node);
};