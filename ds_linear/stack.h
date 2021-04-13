#pragma once

#include "linked_list.h"

template <typename T>
class stack
{
public:
	// Constructors
	stack();
	stack(const stack<T>& prev);
	stack(stack<T>&& prev) noexcept;

	// Destructor
	~stack();

	// Operator overloading
	stack& operator+=(T value);
	stack& operator=(const stack<T>& prev);
	stack& operator=(stack<T>&& prev) noexcept;

	// Other methods
	void push(T value);
	T pop();
	T peak();
	int size() const;
	void clear();

private:
	linked_list<T>* internal_data_;
};