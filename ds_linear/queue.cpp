#include "queue.h"

template <typename T>
queue<T>& queue<T>::operator+=(T value)
{
    enqueue(value);
    return *this;
}

template <typename T>
void queue<T>::enqueue(T value) const
{
    internal_data_.append(value);
}

template <typename T>
T queue<T>::dequeue() const
{
    return internal_data_.remove(0);
}

template <typename T>
T queue<T>::peak() const
{
    return internal_data_[0];
}

template <typename T>
int queue<T>::size() const
{
    return internal_data_.size();
}

template <typename T>
void queue<T>::clear() const
{
    internal_data_.clear();
}