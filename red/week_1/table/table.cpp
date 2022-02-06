#include "test_runner.h"

using namespace std;

// Реализуйте здесь шаблонный класс Table
template <typename T>
class Table {
public:
	Table(size_t h, size_t w): cols(w), rows(h)
	{
		storage.resize(rows);
		for (auto &row: storage){
			row.resize(cols);
		}
	}
	
	void Resize(size_t h, size_t w)
	{
		cols = w;
		rows = h;
		storage.resize(rows);
		for (auto &row: storage){
			row.resize(cols);
		}
	}

	pair<size_t, size_t> Size() const
	{
		if (0 == cols || 0 == rows){
			return make_pair(0, 0);
		} else {
			return make_pair(rows, cols);
		}
	}

	const vector<T> &operator[](size_t id) const
	{
		return storage[id];
	}

	vector<T> &operator[](size_t id) 
	{
		return storage[id];
	}

private:
	size_t cols;
	size_t rows;
	vector<vector<T>> storage;
};

void TestTable() {
  Table<int> t(1, 1);
  ASSERT_EQUAL(t.Size().first, 1u);
  ASSERT_EQUAL(t.Size().second, 1u);
  t[0][0] = 42;
  ASSERT_EQUAL(t[0][0], 42);
  t.Resize(3, 4);
  ASSERT_EQUAL(t.Size().first, 3u);
  ASSERT_EQUAL(t.Size().second, 4u);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestTable);

  return 0;
}
