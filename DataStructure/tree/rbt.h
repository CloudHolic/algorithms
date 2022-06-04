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
    void insert(int value);
    void remove(int value);
    bool exist(int value) const;

    vector<int> inorder() const;
    vector<int> levelorder() const;

    void clear() const;

private:
    enum node_color
    {
        red,
        black
    };

    struct node
    {
        int data;
        node_color color;
        node* left, * right, * parent;

        explicit node(int value);

        node* uncle() const;
        node* sibling() const;

        bool is_left() const;

        void move_down(node* new_parent);
    };

    node* root_;

    // Private methods
    static node* copy(const node* root);
    node* search(int value) const;

    static node* find_replaceable(const node* target);

    void fix_double_red(node* target);
    void fix_double_black(node* target);

    void left_rotate(node* target);
    void right_rotate(node* target);

    static void swap_color(node* target1, node* target2);
    static void swap_value(node* target1, node* target2);

    static void internal_clear(node* root);
};