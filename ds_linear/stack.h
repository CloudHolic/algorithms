#pragma once

#include "linked_list.h"

template <typename T>
class stack
{
public:
	// Operator overloading
	stack& operator+=(T value);

	// Other methods
	void push(const T value) const;
	T pop() const;
	T peak() const;
	int size() const;
	void clear() const;

private:
	linked_list<T> internal_data_;
};