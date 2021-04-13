#pragma once

template <typename T>
class linked_list
{
public:
	// Constructors
	linked_list();
	linked_list(const linked_list<T>& prev);
	linked_list(linked_list<T>&& prev) noexcept;

	// Destructor
	~linked_list();

	// Operator overloading
	linked_list& operator+(const linked_list& add);
	linked_list& operator+=(const linked_list& add);
	linked_list& operator+=(T value);
	T operator[](int index);
	linked_list& operator=(const linked_list<T>& prev);
	linked_list& operator=(linked_list<T>&& prev) noexcept;

	// Other methods
	void insert(int index, T value);
	void append(const linked_list<T>& add);
	void append(T value);
	T remove(int index);
	T at(int index) const;
	T back() const;
	int size() const;
	void clear();

private:
	struct node
	{
		T data;
		node* next_node = nullptr;
	};

	node* head_, * tail_;
	int size_ = 0;

	// Private methods
	node* find_node(int index) const;
	void internal_clear() const;
};