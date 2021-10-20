#pragma once

#include <cstdlib>

// Реализуйте шаблон SimpleVector
template <typename T>
class SimpleVector {
public:
  SimpleVector()
  {
	  capacity = 0;
	  data = NULL;
	  _end = NULL;
  }
  explicit SimpleVector(size_t size)
  {
	  data = new T[size];
	  _end = data + size;
	  capacity = size;
  }
  ~SimpleVector()
  {
	  delete[] data;
  }

  T& operator[](size_t index)
  {
	  return data[index];
  }

  T* begin() {return data;}
  T* end() { return _end;}

  const T* begin() const {return data;}
  const T* end() const { return _end;}

  size_t Size() const
  {
	  return (end() - begin()); 
  }
  size_t Capacity() const
  {
	  return capacity;
  }
  void PushBack(const T& value);

private:
  T *data;
  T *_end;
  size_t capacity;
};
