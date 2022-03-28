#include "test_runner.h"
#include <algorithm>
#include <memory>
#include <vector>
#include <iterator>

using namespace std;

template <typename RandomIt>
void MergeSort(RandomIt range_begin, RandomIt range_end) {
	auto range_size = range_end - range_begin;
	if (range_size < 2){
		return;
	} else {
		vector<typename RandomIt::value_type> v(make_move_iterator(range_begin), make_move_iterator(range_end));
		auto step = range_size / 3;
		MergeSort(range_begin, range_begin + step);
		MergeSort(range_begin + step, range_begin + 2 * step);
		MergeSort(range_begin + 2 * step, range_begin + 3 * step);
		vector<typename RandomIt::value_type> tmp;
		merge(
				make_move_iterator(range_begin), make_move_iterator(range_begin + step),
				make_move_iterator(range_begin + step), make_move_iterator(range_begin + 2 * step),
				back_inserter(tmp)	
				);
		merge(
				make_move_iterator(tmp.begin()), make_move_iterator(tmp.end()),
				make_move_iterator(range_begin + 2 * step), make_move_iterator(range_begin + 3 * step),
				range_begin	
				);
	}
}

void TestIntVector() {
  vector<int> numbers = {6, 1, 3, 9, 1, 9, 8, 12, 1};
  MergeSort(begin(numbers), end(numbers));
  ASSERT(is_sorted(begin(numbers), end(numbers)));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestIntVector);
  return 0;
}
