#pragma once

#include <cstdlib>
#include <iostream>

// Реализуйте шаблон SimpleVector
template <typename T>
class SimpleVector {
public:
  SimpleVector()
  {
	  data = NULL;
	  _end = NULL;
	  capacity = 0;
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
  void PushBack(const T& value){
	  if(Capacity() == Size()){
		  if (Capacity() == 0){
			  capacity = 1;
			  data = new T[capacity];
			  *data = value;
			  _end = data + capacity;
		  } else {
			  size_t old_size = Size();
			  T 	*old_data = data;
			  // Set new block
			  capacity = 2 * old_size;
			  data = new T[capacity];
			  _end = data + old_size;
			  for (int i = 0; i < old_size; i++){
			      data[i] = old_data[i];
			  }
			  delete[] old_data;
			  // Push back
			  *_end = value;
			  _end += 1;
		  }
	  } else {
		  *_end = value;
		  _end += 1;
	  }
  }

private:
  T *data;
  T *_end;
  size_t capacity;
};
