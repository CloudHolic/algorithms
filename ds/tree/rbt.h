#pragma once

#include <vector>

using namespace std;
class rbt
{
public:
    // Constructors
    rbt();
    rbt(const rbt& prev);
    rbt(rbt&& prev) noexcept;

    // Destructor
    ~rbt();

    // Operator overloading
    rbt& operator=(const rbt& prev);
    rbt& operator=(rbt&& prev) noexcept;

    // Other methods
    void insert(const int value);
    void remove(const int value);
    bool exist(const int value) const;
    vector<int> inorder() const;
    vector<int> levelorder() const;
    void clear();

private:
    enum node_color
    {
        RED,
        BLACK
    };
    struct node
    {
        int data;
        node_color color;
        node *left, *right, *parent;

        node(int value);
    };

    node* root_;

    // Private methods
    void internal_clear(node* root);
};