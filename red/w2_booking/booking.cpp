#include <iomanip>
#include <iostream>
#include <map>
#include <algorithm>
#include <utility>
#include <numeric>

using namespace std;
class BookingSystem {
public:
	BookingSystem() {};
private:
};
class ReadingManager {
public:
	ReadingManager() {};

	void Read(int user_id, int page_count) {
		user_page_counts_[user_id] = page_count;
	}

double Cheer(int user_id) const {
	if (user_page_counts_.count(user_id) == 0) {
		return 0;
	}
	const int user_count = GetUserCount();
	if (user_count == 1) {
		return 1;
	}
	int page_count = user_page_counts_.at(user_id);
	auto lbound = page_counts_user_counts.lower_bound(page_count);
	auto part = accumulate(page_counts_user_counts.begin(),
			lbound,
			0,
			[](int sum, const pair<int, int> &item){
				return sum + item.second;
			}
	);
	return part * 1.0 / (user_count - 1);

}

private:
	map<int, int> page_counts_user_counts;
	map<int, int> user_page_counts_;

	int GetUserCount() const {
		return user_page_counts_.size();
	}
};


int main() {
  // Для ускорения чтения данных отключается синхронизация
  // cin и cout с stdio,
  // а также выполняется отвязка cin от cout
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  BookingSystem bsystem;

  int query_count;
  cin >> query_count;

  for (int query_id = 0; query_id < query_count; ++query_id) {
    string query_type;
    cin >> query_type;

    if (query_type == "BOOK") {
      int time;
      cin >> time;
	  string hotel_name;
	  cin >> hotel_name;
	  unsigned int client_id;
	  cin >> client_id;
	  unsigned int room_count;
	  cin >> room_count;

      bsystem.Book(hotel_name, time, client_id, room_count);
    } else if (query_type == "CLIENTS") {
	  string hotel_name;
	  cin >> hotel_name;
      cout << bsystem.Clients(hotel_name) << "\n";
    } else if (query_type == "ROOMS") {
	  string hotel_name;
	  cin >> hotel_name;
      cout << bsystem.Rooms(hotel_name) << "\n";
    } else {
		// Unknown query type
	}

  }

  return 0;
}
