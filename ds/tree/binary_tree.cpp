#include <stack>
#include "binary_tree.h"

template <typename T>
tree_node<T>::tree_node(T value) : data(value), left_child(nullptr), right_child(nullptr) {}

template <typename T>
binary_tree<T>::binary_tree(T root_value)
{
    root_ = new tree_node(root_value);
}

template <typename T>
binary_tree<T>::~binary_tree()
{
    clear(root_);
}

template <typename T>
tree_node<T>* binary_tree<T>::get_root()
{
    return root_;
}

template <typename T>
tree_node<T>* binary_tree<T>::get_child(tree_node<T>* parent_node, bool is_left)
{
    return is_left ? parent_node->left_child : parent_node->right_child;
}

template <typename T>
void binary_tree<T>::insert(tree_node<T>* parent_node, T value, bool is_left)
{
    tree_node<T>* new_node = new tree_node(value);
    if (is_left)
        parent_node->left_child = new_node;
    else
        parent_node->right_child = new_node;
}

template <typename T>
vector<T> binary_tree<T>::preorder() const
{
    vector<T> result;
    if (root_ == nullptr)
        return result;

    stack<tree_node<T>*> node_stack;
    node_stack.push(root_);

    while (!node_stack.empty())
    {
        tree_node<T>* source = node_stack.top();
        node_stack.pop();
        result.push_back(source->data);

        if (source->right_child != nullptr)
            node_stack.push(source->right_child);
        if (source->left_child != nullptr)
            node_stack.push(source->left_child);
    }

    return result;
}

template <typename T>
vector<T> binary_tree<T>::inorder() const
{
    vector<T> result;
    if (root_ == nullptr)
        return result;

    stack<tree_node<T>*> node_stack;
    tree_node<T>* cur_node = root_;

    while (cur_node != nullptr || !node_stack.empty())
    {
        while (cur_node != nullptr)
        {
            node_stack.push(cur_node);
            cur_node = cur_node->left_child;
        }

        cur_node = node_stack.top();
        node_stack.pop();
        result.push_back(cur_node->data);

        cur_node = cur_node->right_child;
    }

    return result;
}

template <typename T>
vector<T> binary_tree<T>::postorder() const
{
    vector<T> result;
    if (root_ == nullptr)
        return result;

    stack<tree_node<T>*> node_stack;
    tree_node<T>* cur_node = root_;

    while (!node_stack.empty())
    {
        while (cur_node != nullptr)
        {
            node_stack.push(cur_node);
            node_stack.push(cur_node);
            cur_node = cur_node->left_child;
        }

        cur_node = node_stack.top();
        node_stack.pop();

        if (node_stack.top() == cur_node)
            cur_node = cur_node->right_child;
        else
        {
            result.push_back(cur_node->data);
            cur_node = nullptr;
        }
    }
    
    return result;
}

template <typename T>
void binary_tree<T>::clear(tree_node<T>* node)
{
    if (node != nullptr)
    {
        clear(node->left_child);
        clear(node->right_child);
        delete node;
    }
}