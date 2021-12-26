#include "test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
using namespace std;

template <class T>
class ObjectPool {
public:
  T* Allocate()
  {
	  if (!free.empty()){
		  T *res = free.front();
		  free.pop();
		  allocated.insert(res);
		  return res;
	  } else {
		  T *res = new T;
		  allocated.insert(res);
		  return res;
	  }
  }

  T* TryAllocate()
  {
	  if (!free.empty()){
		  T *res = free.front();
		  free.pop();
		  allocated.insert(res);
		  return res;
	  } else {
		  return nullptr;
	  }
  }

  void Deallocate(T* object)
  {
	  if (allocated.count(object) == 0){
		  throw invalid_argument("Can't deallocate such an object");
	  } else {
		  auto it = allocated.find(object);
		  free.push(*it);
		  allocated.erase(it);
	  }
  }

  ~ObjectPool()
  {
	  size_t free_size = free.size();
	  for (int i = 0; i < free_size; ++i){
		  delete free.front();
		  free.pop();
	  }
	  for (auto &item: allocated){
		  delete item;
	  }
	  allocated.clear();
  }

private:
  queue<T*> free;
  set<T*> allocated;
};

void TestObjectPool() {
  ObjectPool<string> pool;

  auto p1 = pool.Allocate();
  auto p2 = pool.Allocate();
  auto p3 = pool.Allocate();

  *p1 = "first";
  *p2 = "second";
  *p3 = "third";

  pool.Deallocate(p2);
  ASSERT_EQUAL(*pool.Allocate(), "second");

  pool.Deallocate(p3);
  pool.Deallocate(p1);
  ASSERT_EQUAL(*pool.Allocate(), "third");
  ASSERT_EQUAL(*pool.Allocate(), "first");

  pool.Deallocate(p1);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestObjectPool);
  return 0;
}
