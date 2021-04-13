#pragma once

#include "linked_list.h"

template <typename T>
class queue
{
public:
    // Operator overloading
    queue& operator+=(T value);

    // Other methods
    void enqueue(T value) const;
    T dequeue() const;
    T peak() const;
    int size() const;
    void clear() const;

private:
    linked_list<T> internal_data_;
};