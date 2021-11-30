#pragma once

#include <stdexcept>
#include <array>
using namespace std;

template <typename T, size_t N>
class StackVector {
public:
	explicit StackVector(size_t a_size = 0)
	{
		if (a_size > N){
			throw(invalid_argument("Size is bigger than capacity"));
		} else {
			_size = a_size;
		}
	}

	T& operator[](size_t index);
	const T& operator[](size_t index) const;

	auto begin();
	auto end();
	auto begin() const;
	auto end() const;

	size_t Size() const;
	size_t Capacity() const;

	void PushBack(const T& value);
	T PopBack();

private:
	array<T, N> data;
	size_t _size;
	size_t _capacity = N;
};

template <typename T, size_t N>
size_t StackVector<T, N>::Capacity() const
{
	return _capacity;
}

template <typename T, size_t N>
size_t StackVector<T, N>::Size() const
{
	return _size;
}

template <typename T, size_t N>
auto StackVector<T, N>::begin()
{
	return data.begin();
}

template <typename T, size_t N>
auto StackVector<T, N>::end()
{
	return (data.begin() + _size);
}

template <typename T, size_t N>
auto StackVector<T, N>::begin() const
{
	return data.begin();
}

template <typename T, size_t N>
auto StackVector<T, N>::end() const
{
	return (data.begin() + _size);
}


template <typename T, size_t N>
T& StackVector<T, N>::operator[](size_t index)
{
	return data[index];
}

template <typename T, size_t N>
const T& StackVector<T, N>::operator[](size_t index) const
{
	return data[index];
}

template <typename T, size_t N>
void StackVector<T, N>::PushBack(const T& value)
{
	if (_size == _capacity){
		throw(overflow_error("The vector is full"));
	} else {
		data[_size] = value;
		_size++;
	}
}

template <typename T, size_t N>
T StackVector<T, N>::PopBack()
{
	if (_size == 0){
		throw(underflow_error("The vector is empty"));
	} else {
		_size--;
		return data[_size];
	}
}

