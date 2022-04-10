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
  using Id = int/* тип, используемый для идентификаторов */;

  // Добавить объект с нулевым приоритетом
  // с помощью перемещения и вернуть его идентификатор
  Id Add(T object)
  {
	  id_to_objects[id] = make_pair(move(object), 0);
	  pr_to_ids[0].insert(id);
	  return id++;
  }

  // Добавить все элементы диапазона [range_begin, range_end)
  // с помощью перемещения, записав выданные им идентификаторы
  // в диапазон [ids_begin, ...)
  template <typename ObjInputIt, typename IdOutputIt>
  void Add(ObjInputIt range_begin, ObjInputIt range_end,
           IdOutputIt ids_begin)
  {
	  for(auto it = range_begin; it != range_end; it = next(it)){
		  *ids_begin++ = Add(move(*it));
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
	return (id_to_objects.at(id).first);

  }

  // Увеличить приоритет объекта на 1
  void Promote(Id id)
  {
	  // Save the priority and increase it by 1
	int old_priority = id_to_objects[id].second;
	id_to_objects[id].second++;
	// Delete the object from the old priority's set
	pr_to_ids[old_priority].erase(id);
	if (pr_to_ids[old_priority].size() == 0){
		pr_to_ids.erase(old_priority);
	}
	// And promote it to the next set
	pr_to_ids[old_priority + 1].insert(id);
  }

  // Получить объект с максимальным приоритетом и его приоритет
  pair<const T&, int> GetMax() const
  {
	  // Get the set of objects having a max priority
	  set<Id> max_pr_set = (*pr_to_ids.rbegin()).second;
	  // Get an ID of the newest object (it has max id)
	  int max_id = *max_pr_set.rbegin();
	  return id_to_objects.at(max_id);

  }

  // Аналогично GetMax, но удаляет элемент из контейнера
  pair<T, int> PopMax()
  {
	  // Get the set of objects having a max priority
	  set<Id> &max_pr_set = (*pr_to_ids.rbegin()).second;
	  // Get an ID of the newest object (it has max id)
	  int max_id = *(max_pr_set.rbegin());
	  // Create return value before the deleting of the element
	  auto ret = make_pair(move(id_to_objects.at(max_id).first), id_to_objects.at(max_id).second);
	  // Delete the object
	  id_to_objects.erase(max_id);
	  max_pr_set.erase(max_id);
	  if (max_pr_set.size() == 0){
		  pr_to_ids.erase(prev(pr_to_ids.end()));
	  }
	  return ret;
  }

private:
  // Find object and promote it
  map<Id, pair<T, int>> id_to_objects;
  // Find the newest object with a max priority
  map<int, set<Id>> pr_to_ids;
  int id = 0;
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
