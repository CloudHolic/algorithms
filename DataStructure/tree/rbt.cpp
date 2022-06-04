#include <queue>
#include <stack>
#include "rbt.h"

rbt::rbt() : root_(nullptr) {}

rbt::rbt(const rbt& prev) : root_(nullptr)
{
    root_ = copy(prev.root_);
}

rbt::rbt(rbt&& prev) noexcept : root_(nullptr)
{
    *this = std::move(prev);
}

rbt::~rbt()
{
    internal_clear(root_);
}

rbt& rbt::operator=(const rbt& prev)
{
    if (this != &prev)
    {
        clear();
        root_ = copy(prev.root_);
    }

    return *this;
}

rbt& rbt::operator=(rbt&& prev) noexcept
{
    if (this != &prev)
    {
        clear();

        std::swap(root_, prev.root_);

        if (root_->left != nullptr)
            root_->left->parent = root_;
        if (root_->right != nullptr)
            root_->right->parent = root_;
    }

    return *this;
}

void rbt::insert(const int value)
{
	const auto new_node = new node(value);
    if (root_ == nullptr)
    {
        new_node->color = black;
        root_ = new_node;
    }
    else
    {
        node* target = search(value);

        if (target->data == value)
            return;

        new_node->parent = target;
        if (value < target->data)
            target->left = new_node;
        else
            target->right = new_node;

        fix_double_red(new_node);
    }
}

void rbt::remove(const int value)
{
    if (root_ == nullptr)
        return;

    node* target = search(value);
    if (target->data != value)
        return;

    node* replace = find_replaceable(target);
    node* parent = target->parent;

    const bool both_black = target->color == black && (replace == nullptr || replace->color == black);

    if (replace == nullptr)
    {
        if (target == root_)
            root_ = nullptr;
        else
        {
            if (both_black)
                fix_double_black(target);
            else
                if (target->sibling() != nullptr)
                    target->sibling()->color = red;

            if (target->is_left())
                parent->left = nullptr;
            else
                parent->right = nullptr;
        }

        delete target;
    }
    else if (replace->left == nullptr || replace->right == nullptr)
    {
        if (target == root_)
        {
            target->data = replace->data;
            target->left = target->right = nullptr;
            delete replace;
        }
        else
        {
            if (target->is_left())
                parent->left = replace;
            else
                parent->right = replace;

            delete target;
            replace->parent = parent;

            if (both_black)
                fix_double_black(replace);
            else
                replace->color = black;
        }
    }
    else
    {
        swap_value(target, replace);
        remove(value);
    }
}

bool rbt::exist(const int value) const
{
    return search(value)->data == value;
}

vector<int> rbt::inorder() const
{
    vector<int> result;
    if (root_ == nullptr)
        return result;

    stack<node*> node_stack;
    node* cur_node = root_;

    while (cur_node != nullptr || !node_stack.empty())
    {
        while (cur_node != nullptr)
        {
            node_stack.push(cur_node);
            cur_node = cur_node->left;
        }

        cur_node = node_stack.top();
        node_stack.pop();
        result.push_back(cur_node->data);

        cur_node = cur_node->right;
    }

    return result;
}

vector<int> rbt::levelorder() const
{
    vector<int> result;
    if (root_ == nullptr)
        return result;

    queue<node*> node_queue;
    node* cur_node = root_;

    node_queue.push(cur_node);
    while (!node_queue.empty())
    {
        cur_node = node_queue.front();
        node_queue.pop();

        result.push_back(cur_node->data);

        if (cur_node->left != nullptr)
            node_queue.push(cur_node->left);
        if (cur_node->right != nullptr)
            node_queue.push(cur_node->right);
    }

    return result;
}

void rbt::clear() const
{
    internal_clear(root_);
}

rbt::node* rbt::copy(const node* root)
{
	const auto new_node = new node(root->data);

    if (root->left != nullptr)
    {
        new_node->left = copy(root->left);
        new_node->left->parent = new_node;
    }
    if (root->right != nullptr)
    {
        new_node->right = copy(root->right);
        new_node->right->parent = new_node;
    }

    return new_node;
}

rbt::node* rbt::search(const int value) const
{
	node* temp = root_;
    while (temp != nullptr)
    {
        if (value < temp->data)
        {
            if (temp->left == nullptr)
                break;
            temp = temp->left;
        }
        else if (value > temp->data)
        {
            if (temp->right == nullptr)
                break;
            temp = temp->right;
        }
        else
            break;
    }

    return temp;
}

rbt::node* rbt::find_replaceable(const node* target)
{
    if (target->left != nullptr && target->right != nullptr)
    {
        node* temp = target->right;
        while (temp->left != nullptr)
            temp = temp->left;

        return temp;
    }

    if (target->left == nullptr && target->right == nullptr)
        return nullptr;

    if (target->left != nullptr)
        return target->left;
    else
        return target->right;
}

void rbt::fix_double_red(node* target)
{
    if (target == root_)
    {
        target->color = black;
        return;
    }

    node* parent = target->parent;
    node* grandparent = target->parent->parent;
    node* uncle = target->uncle();

    if (parent->color != black)
    {
        if (uncle != nullptr && uncle->color == red)
        {
            parent->color = black;
            uncle->color = black;
            grandparent->color = red;
            fix_double_red(grandparent);
        }
        else
        {
            if (parent->is_left())
            {
                if (target->is_left())
                    swap_color(parent, grandparent);
                else
                {
                    left_rotate(parent);
                    swap_color(target, grandparent);
                }

                right_rotate(grandparent);
            }
            else
            {
                if (target->is_left())
                {
                    right_rotate(parent);
                    swap_color(target, grandparent);
                }
                else
                    swap_color(parent, grandparent);

                left_rotate(grandparent);
            }
        }
    }
}

void rbt::fix_double_black(node* target)
{
    if (target == root_)
        return;

    node* parent = target->parent;
    node* sibling = target->sibling();

    if (sibling == nullptr)
        fix_double_black(parent);
    else
    {
        if (sibling->color == red)
        {
            parent->color = red;
            sibling->color = black;

            if (sibling->is_left())
                right_rotate(parent);
            else
                left_rotate(parent);

            fix_double_black(target);
        }
        else
        {
            if (sibling->left != nullptr && sibling->left->color == red)
            {
                if (sibling->is_left())
                {
                    sibling->left->color = sibling->color;
                    sibling->color = parent->color;
                    right_rotate(parent);
                }
                else
                {
                    sibling->left->color = parent->color;
                    right_rotate(sibling);
                    left_rotate(parent);
                }

                parent->color = black;
            }
            else if (sibling->right != nullptr && sibling->right->color == red)
            {
                if (sibling->is_left())
                {
                    sibling->right->color = parent->color;
                    left_rotate(sibling);
                    right_rotate(parent);
                }
                else
                {
                    sibling->right->color = sibling->color;
                    sibling->color = parent->color;
                    left_rotate(parent);
                }

                parent->color = black;
            }
            else
            {
                sibling->color = red;
                if (parent->color == black)
                    fix_double_black(parent);
                else
                    parent->color = black;
            }
        }
    }
}

void rbt::left_rotate(node* target)
{
    node* new_parent = target->right;

    if (target == root_)
        root_ = new_parent;

    target->move_down(new_parent);

    target->right = new_parent->left;
    if (new_parent != nullptr && new_parent->left != nullptr)
        new_parent->left->parent = target;

    new_parent->left = target;
}

void rbt::right_rotate(node* target)
{
    node* new_parent = target->left;

    if (target == root_)
        root_ = new_parent;

    target->move_down(new_parent);

    target->left = new_parent->right;
    if (new_parent != nullptr && new_parent->right != nullptr)
        new_parent->right->parent = target;

    new_parent->right = target;
}

void rbt::swap_color(node* target1, node* target2)
{
	const node_color temp = target1->color;
    target1->color = target2->color;
    target2->color = temp;
}

void rbt::swap_value(node* target1, node* target2)
{
	const int temp = target1->data;
    target1->data = target2->data;
    target2->data = temp;
}

void rbt::internal_clear(node* root)
{
    if (root->left != nullptr)
        internal_clear(root->left);
    if (root->right != nullptr)
        internal_clear(root->right);

    if (root->parent != nullptr)
    {
        if (root->parent->left == root)
            root->parent->left = nullptr;
        else if (root->parent->right == root)
            root->parent->right = nullptr;

        root->parent = nullptr;
    }

    delete root;
}

rbt::node::node(int value) : data(value)
{
    left = right = parent = nullptr;
    color = red;
}

rbt::node* rbt::node::uncle() const
{
    if (parent == nullptr || parent->parent == nullptr)
        return nullptr;

    if (parent->is_left())
        return parent->parent->right;
    return parent->parent->left;
}

rbt::node* rbt::node::sibling() const
{
    if (parent == nullptr)
        return nullptr;

    if (is_left())
        return parent->right;

    return parent->left;
}

bool rbt::node::is_left() const
{
    return this == parent->left;
}

void rbt::node::move_down(node* new_parent)
{
    if (parent != nullptr)
    {
        if (is_left())
            parent->left = new_parent;
        else
            parent->right = new_parent;
    }

    new_parent->parent = parent;
    parent = new_parent;
}