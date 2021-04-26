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

        node* uncle();
        node* sibling();
        
        bool is_left();

        void move_down(node* new_parent);
    };

    node* root_;

    // Private methods
    node* copy(const node* root) const;
    node* search(const int value) const;

    node* find_replaceable(node* target);
    
    void fix_double_red(node* target);
    void fix_double_black(node* target);

    void left_rotate(node* target);
    void right_rotate(node* target);

    void swap_color(node* target1, node* target2);
    void swap_value(node* target1, node* target2);

    void internal_clear(node* root);
};