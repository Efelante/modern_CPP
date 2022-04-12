#include "test_runner.h"
#include <vector>
#include <future>

#include <numeric>
#include <iostream>
#include <string>
#include <utility>
#include <iterator>

using namespace std;

template <typename Iterator>
class Page {
public:
	Page(Iterator begin, Iterator end)
	: _begin(begin), _end(end)
	{
		// Empty body
	}

	Iterator begin()
	{
		return _begin;
	}

	Iterator end()
	{
		return _end;
	}

	Iterator begin() const
	{
		return _begin;
	}

	Iterator end() const
	{
		return _end;
	}


	size_t size() const
	{
		return (_end - _begin);
	}
private:
	Iterator _begin;
	Iterator _end;
};

// Реализуйте шаблон класса Paginator

template <typename Iterator>
class Paginator {
public:
	Paginator(Iterator begin, Iterator end, size_t page_size)
	{
		Iterator page_start = begin;
		Iterator page_end;
		auto container_size = end - begin;
		size_t full_count = container_size / page_size;
		for (size_t i = 0; i < full_count; ++i){
			page_end = page_start + page_size;
			pages.push_back(Page(page_start, page_end));
			page_start = page_end;
		}
		if (container_size % page_size){
			pages.push_back(Page(page_start, end));
		}
	}

	size_t size() const
	{
		return pages.size();
	}

	auto begin()
	{
		return pages.begin();
	}
	
	auto end()
	{
		return pages.end();
	}
	// Why don't I need const begin and end
	// Because of auto?
private:
	vector<Page<Iterator>> pages;
};

template <typename C>
auto Paginate(C& c, size_t page_size) {
  return Paginator{c.begin(), c.end(), page_size};
}

int64_t CalculateMatrixSum(const vector<vector<int>>& matrix) {
	const size_t page_size = 2250;
	vector<future<int64_t>> part_sums;
	for(auto page: Paginate(matrix, page_size)){
		part_sums.push_back(async([page](){
				int64_t res = 0;
				for(const auto &row: page){
					res += accumulate(row.begin(), row.end(), 0);
				}
				return res;
			}));
	}
	int64_t res = 0;
	for (auto &part_sum: part_sums){
		res += part_sum.get();
	}
	return res;
}

void TestCalculateMatrixSum() {
  const vector<vector<int>> matrix = {
    {1, 2, 3, 4},
    {5, 6, 7, 8},
    {9, 10, 11, 12},
    {13, 14, 15, 16}
  };
  ASSERT_EQUAL(CalculateMatrixSum(matrix), 136);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestCalculateMatrixSum);
}
