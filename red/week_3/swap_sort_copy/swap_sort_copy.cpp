#include "test_runner.h"

#include <algorithm>
#include <numeric>
using namespace std;

template <typename T>
void Swap(T* first, T* second)
{
	T buf = *first;
	*first = *second;
	*second = buf;
}

template <typename T>
void SortPointers(vector<T*>& pointers)
{
	for (size_t i = 0; i < pointers.size() - 1; ++i){
		for (size_t j = i + 1; j < pointers.size(); ++j){
			if (*pointers[j] < *pointers[i]){
				Swap(pointers[i], pointers[j]);
			} else {
				// Don't swap
			}
		}
	}
}

template <typename T>
void ReversedCopy(T* src, size_t count, T* dst)
{
	if (dst >= src && dst < (src + count) || (dst + count - 1) >= src && (dst + count - 1)< (src + count)){
		if (dst < src){
			int n = dst + count - src;
			for (int i = 0; i < count - n; ++i){
				dst[i] = src[count - 1 - i];
			}
			for (int i = 0; i < n/2; ++i){
				Swap(src + n - 1 - i, dst + count - n + i);
			}
		} else {
			int n = src + count - dst;
			for (int i = 0; i < n/2; ++i){
				Swap(src + count - 1 - i, dst + i);
			}
			for (int i = 0; i < count - n; ++i){
				dst[n + i] = src[count - 1 - n - i];
			}
		}
	} else {
		for (int i = 0; i < count; ++i){
			dst[i]= src[count - 1 - i];
		}


	}
}

void TestSwap() {
  int a = 1;
  int b = 2;
  Swap(&a, &b);
  ASSERT_EQUAL(a, 2);
  ASSERT_EQUAL(b, 1);

  string h = "world";
  string w = "hello";
  Swap(&h, &w);
  ASSERT_EQUAL(h, "hello");
  ASSERT_EQUAL(w, "world");
}

void TestSortPointers() {
  int one = 1;
  int two = 2;
  int three = 3;

  vector<int*> pointers;
  pointers.push_back(&two);
  pointers.push_back(&three);
  pointers.push_back(&one);

  SortPointers(pointers);

  ASSERT_EQUAL(pointers.size(), 3u);
  ASSERT_EQUAL(*pointers[0], 1);
  ASSERT_EQUAL(*pointers[1], 2);
  ASSERT_EQUAL(*pointers[2], 3);
}

void TestReverseCopy() {
  const size_t count = 7;

  int* src = new int[count];
  int* dest = new int[count];

  for (size_t i = 0; i < count; ++i) {
    src[i] = i + 1;
  }
  ReversedCopy(src, count, dest);
  const vector<int> expected1 = {7, 6, 5, 4, 3, 2, 1};
  ASSERT_EQUAL(vector<int>(dest, dest + count), expected1);

  // Области памяти могут перекрываться
  ReversedCopy(src, count - 1, src + 1);
  const vector<int> expected2 = {1, 6, 5, 4, 3, 2, 1};
  ASSERT_EQUAL(vector<int>(src, src + count), expected2);

  // Области памяти могут перекрываться - V2
  for (size_t i = 0; i < count; ++i) {
    src[i] = i + 1;
  }
  ReversedCopy(src + 1, count - 2, src);
  const vector<int> expected3 = {6, 5, 4, 3, 2, 6, 7};
  ASSERT_EQUAL(vector<int>(src, src + count), expected3);


  delete[] dest;
  delete[] src;
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSwap);
  RUN_TEST(tr, TestSortPointers);
  RUN_TEST(tr, TestReverseCopy);
  return 0;
}
