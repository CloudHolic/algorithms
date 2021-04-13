#include <stdexcept>
#include "linked_list.h"

template <typename T>
linked_list<T>::linked_list() : head_(nullptr), tail_(nullptr) {}

template <typename T>
linked_list<T>::linked_list(const linked_list<T>& prev) : head_(nullptr), tail_(nullptr)
{
	for (int i = 0; i < prev.size(); i++)
		append(prev.at(i));
}

template <typename T>
linked_list<T>::linked_list(linked_list<T>&& prev) noexcept : head_(nullptr), tail_(nullptr)
{
	*this = std::move(prev);
}

template <typename T>
linked_list<T>::~linked_list()
{
	internal_clear();
}

template <typename T>
linked_list<T>& linked_list<T>::operator=(const linked_list<T>& prev)
{
	if (this != &prev)
	{
		clear();

		for (int i = 0; i < prev.size(); i++)
			append(prev.at(i));
	}

	return *this;
}

template <typename T>
linked_list<T>& linked_list<T>::operator=(linked_list<T>&& prev) noexcept
{
	if (this != &prev)
	{
		clear();

		std::swap(head_, prev.head_);
		std::swap(tail_, prev.tail_);
		std::swap(size_, prev.size_);
	}

	return *this;
}

template <typename T>
linked_list<T>& linked_list<T>::operator+(const linked_list& add)
{
	linked_list<T> new_list = linked_list<T>(*this);
	new_list += add;
	return new_list;
}

template <typename T>
linked_list<T>& linked_list<T>::operator+=(const linked_list& add)
{
	append(add);
	return *this;
}

template <typename T>
linked_list<T>& linked_list<T>::operator+=(T value)
{
	append(value);
	return *this;
}

template <typename T>
T linked_list<T>::operator[](const int index)
{
	return at(index);
}

template <typename T>
void linked_list<T>::insert(const int index, const T value)
{
	if (index > size() - 1 || index < 0)
		throw std::invalid_argument("Invalid index");

	node* new_node = new node();
	new_node->data = value;

	if (index == 0)
	{
		new_node->next_node = head_;
		head_ = new_node;
		tail_ = new_node;
	}
	else
	{
		node* target = find_node(index - 1);
		node* next = target->next_node;

		target->next_node = new_node;
		new_node->next_node = next;
	}

	size_++;
}

template <typename T>
void linked_list<T>::append(const linked_list<T>& add)
{
	for (int i = 0; i < add.size(); i++)
		append(add.at(i));
}

template <typename T>
void linked_list<T>::append(const T value)
{
	node* new_node = new node();
	new_node->data = value;

	if (size() == 0)
		head_ = tail_ = new_node;
	else
	{
		tail_->next_node = new_node;
		tail_ = new_node;
	}
	size_++;
}

template <typename T>
T linked_list<T>::remove(const int index)
{
	T result;
	if (index > size() - 1 || index < 0)
		throw std::invalid_argument("Invalid index");

	if (index == 0)
	{
		node* current = head_->next_node;

		result = head_->data;
		delete head_;
		head_ = current;
	}
	else
	{
		node* target = find_node(index - 1);
		node* next = target->next_node;

		result = next->data;
		target->next_node = next->next_node;
		if (target->next_node == nullptr)
			tail_ = target;
		delete next;
	}

	size_--;
	return result;
}

template <typename T>
T linked_list<T>::at(const int index) const
{
	if (index > size() - 1 || index < 0)
		throw std::invalid_argument("Invalid index");

	if (index == size() - 1)
		return tail_->data;

	return find_node(index)->data;
}

template <typename T>
T linked_list<T>::back() const
{
	return tail_->data;
}

template <typename T>
int linked_list<T>::size() const
{
	return size_;
}

template <typename T>
void linked_list<T>::clear()
{
	internal_clear();
	head_ = tail_ = nullptr;
	size_ = 0;
}

template <typename T>
typename linked_list<T>::node* linked_list<T>::find_node(const int index) const
{
	node* current = head_;
	for (int i = 0; i < index; i++)
		current = head_->next_node;

	return current;
}

template <typename T>
void linked_list<T>::internal_clear() const
{
	node* current = head_;

	while (current != nullptr)
	{
		node* next = current->next_node;
		delete current;
		current = next;
	}
}
