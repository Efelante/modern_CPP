#pragma once

#include <cstdlib>

// Реализуйте шаблон SimpleVector
template <typename T>
class SimpleVector {
public:
  SimpleVector()
  {
	  data = _end;
  }
  explicit SimpleVector(size_t size)
  {
	  data = new T[size];
	  _end = data + size;
  }
  ~SimpleVector();

  T& operator[](size_t index)
  {
	  return data[index];
  }

  T* begin() {return data};
  T* end() { return _end};

  size_t Size() const
  {
	  return 
  }
  size_t Capacity() const;
  void PushBack(const T& value);

private:
  T *data;
  T *_end;
  // Добавьте поля для хранения данных вектора
};
