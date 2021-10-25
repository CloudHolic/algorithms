#include "stack.h"

template <typename T>
stack<T>& stack<T>::operator+=(T value)
{
    push(value);
    return *this;
}

template <typename T>
void stack<T>::push(const T value) const
{
    internal_data_.insert(0, value);
}

template <typename T>
T stack<T>::pop() const
{
    return internal_data_.remove(0);
}

template <typename T>
T stack<T>::peak() const
{
    return internal_data_[0];
}

template <typename T>
int stack<T>::size() const
{
    return internal_data_.size();
}

template <typename T>
void stack<T>::clear() const
{
    internal_data_.clear();
}