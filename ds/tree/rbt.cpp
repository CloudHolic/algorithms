#include "rbt.h"

rbt::rbt() : root_(nullptr) {}

rbt:: rbt(const rbt& prev) : root_(nullptr)
{

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
    if(this != &prev)
    {
        clear();
    }

    return *this;
}

rbt& rbt::operator=(rbt&& prev) noexcept
{
    if(this != &prev)
    {
        clear();

        std::swap(root_, prev.root_);

        if(root_->left != nullptr)
            root_->left->parent = root_;
        if(root_->right != nullptr)
            root_->right->parent = root_;
    }

    return *this;
}

void rbt::insert(const int value)
{

}

void rbt::remove(const int value)
{

}

bool rbt::exist(const int value) const
{

}

vector<int> rbt::inorder() const
{

}

vector<int> rbt::levelorder() const
{

}

void rbt::clear()
{
    internal_clear(root_);
}

rbt::node::node(int value) : data(value)
{
    left = right = parent = nullptr;
    color = RED;
}

void rbt::internal_clear(node* root)
{
    if(root->left != nullptr)
        internal_clear(root->left);
    if(root->right != nullptr)
        internal_clear(root->right);

    if(root->parent != nullptr)
    {
        if(root->parent->left == root)
            root->parent->left = nullptr;
        else if(root->parent->right == root)
            root->parent->right = nullptr;

        root->parent = nullptr;
    }

    delete root;
}