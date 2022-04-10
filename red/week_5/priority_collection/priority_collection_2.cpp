#include "test_runner.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <set>
#include <utility>
#include <vector>
#include <map>

using namespace std;

template <typename T>
class PriorityCollection {
public:
  using Id = size_t/* тип, используемый для идентификаторов */;

  // Добавить объект с нулевым приоритетом
  // с помощью перемещения и вернуть его идентификатор
  Id Add(T object)
  {
	  Id ret = id;
	  id_to_objects[id] = make_pair(move(object), 0);
	  pr_to_ids[0].insert(id);
	  id++;
	  return ret;
  }

  // Добавить все элементы диапазона [range_begin, range_end)
  // с помощью перемещения, записав выданные им идентификаторы
  // в диапазон [ids_begin, ...)
  template <typename ObjInputIt, typename IdOutputIt>
  void Add(ObjInputIt range_begin, ObjInputIt range_end,
           IdOutputIt ids_begin)
  {
	  for(auto it = range_begin; it != range_end; it = next(it)){
		  *ids_begin = Add(*it);
		  ids_begin = next(ids_begin);
	  }

  }

  // Определить, принадлежит ли идентификатор какому-либо
  // хранящемуся в контейнере объекту
  bool IsValid(Id id) const
  {
	return (id_to_objects.find(id) != id_to_objects.end());
  }

  // Получить объект по идентификатору
  const T& Get(Id id) const
  {
	return (id_to_objects[id].first);

  }

  // Увеличить приоритет объекта на 1
  void Promote(Id id)
  {
	size_t prev_pr = id_to_objects[id].second;
	id_to_objects[id].second++;
	pr_to_ids[prev_pr].erase(id);
	if (pr_to_ids[prev_pr].size() == 0){
		pr_to_ids.erase(prev_pr);
	}
	pr_to_ids[prev_pr + 1].insert(id);
  }

  // Получить объект с максимальным приоритетом и его приоритет
  pair<const T&, int> GetMax() const
  {
	  set<Id> max_pr_set = (*pr_to_ids.rbegin()).second;
	  size_t max_id = *max_pr_set.rbegin();
	  return (make_pair(id_to_objects.at(max_id).first, id_to_objects.at(max_id).second));

  }

  // Аналогично GetMax, но удаляет элемент из контейнера
  pair<T, int> PopMax()
  {
	  set<Id> &max_pr_set = (*pr_to_ids.rbegin()).second;
	  size_t max_id = *(max_pr_set.rbegin());
	  auto ret = make_pair(move(id_to_objects.at(max_id).first), id_to_objects.at(max_id).second);
	  id_to_objects.erase(max_id);
	  max_pr_set.erase(max_id);
	  if (max_pr_set.size() == 0){
		  pr_to_ids.erase(prev(pr_to_ids.end()));
	  }
	  return ret;


  }

private:
  // Find object and promote it
  map<Id, pair<T, size_t>> id_to_objects;
  // Find the newest object with a max priority
  map<size_t, set<Id>> pr_to_ids;
  size_t id = 0;
};

class StringNonCopyable : public string {
public:
  using string::string;  // Позволяет использовать конструкторы строки
  StringNonCopyable(const StringNonCopyable&) = delete;
  StringNonCopyable(StringNonCopyable&&) = default;
  StringNonCopyable& operator=(const StringNonCopyable&) = delete;
  StringNonCopyable& operator=(StringNonCopyable&&) = default;
};

void TestNoCopy() {
  PriorityCollection<StringNonCopyable> strings;
  const auto white_id = strings.Add("white");
  const auto yellow_id = strings.Add("yellow");
  const auto red_id = strings.Add("red");

  strings.Promote(yellow_id);
  for (int i = 0; i < 2; ++i) {
    strings.Promote(red_id);
  }
  strings.Promote(yellow_id);
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "red");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "yellow");
    ASSERT_EQUAL(item.second, 2);
  }
  {
    const auto item = strings.PopMax();
    ASSERT_EQUAL(item.first, "white");
    ASSERT_EQUAL(item.second, 0);
  }
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestNoCopy);
  return 0;
}
