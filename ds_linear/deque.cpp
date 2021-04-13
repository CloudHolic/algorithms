#include "deque.h"

template <typename T>
void deque<T>::push_front(T value) const
{
    internal_data_.insert(0, value);
}

template <typename T>
void deque<T>::push_back(T value) const
{
    internal_data_.append(value);
}

template <typename T>
T deque<T>::pop_front() const
{
    return internal_data_.remove(0);
}

template <typename T>
T deque<T>::pop_back() const
{
    return internal_data_.remove(internal_data_.size() - 1);
}

template <typename T>
T deque<T>::peak_front() const
{
    return internal_data_[0];
}

template <typename T>
T deque<T>::peak_back() const
{
    return internal_data_[internal_data_.size() - 1];
}

template <typename T>
int deque<T>::size() const
{
    return internal_data_.size();
}

template <typename T>
void deque<T>::clear() const
{
    internal_data_.clear();
}