#pragma once

#include "linked_list.h"

template <typename T>
class deque
{
public:
    void push_front(T value) const;
    void push_back(T value) const;
    
    T pop_front() const;
    T pop_back() const;

    T peak_front() const;
    T peak_back() const;

    int size() const;
    void clear() const;

private:
    linked_list<T> internal_data_;
};