#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include <utility>
#include <numeric>

using namespace std;
class BookingSystem {
public:
	BookingSystem() {};
	unsigned int Clients(const string &hotel_name) const
	{
		unsigned int res = 0;
		if (storage.count(hotel_name) != 0){
			auto hotel_data = storage.at(hotel_name);
			auto start_time = current_time - 86400;
			auto start_item = hotel_data.upper_bound(start_time);
			set<unsigned int> clients;
			for (auto item = start_item; item != hotel_data.end(); item = next(item)){
				clients.insert(item->second.second.begin(), item->second.second.end());
			}
			res = clients.size();
		}
		return res;
	}

	unsigned int Rooms(const string &hotel_name) const
	{
		unsigned int res = 0;
		if (storage.count(hotel_name) != 0){
			auto hotel_data = storage.at(hotel_name);
			auto start_time = current_time - 86400;
			auto start_item = hotel_data.upper_bound(start_time);
			unsigned int rooms = 0;
			for (auto item = start_item; item != hotel_data.end(); item = next(item)){
				rooms += item->second.first;
			}
			res = rooms;
		}
		return res;
	}

    void Book(const string &hotel_name, long long int time, unsigned int client_id, unsigned int room_count)
	{
		current_time = time;
		storage[hotel_name][time].first += room_count;
		storage[hotel_name][time].second.insert(client_id); 
	}
private:
	long long int current_time = 0;
	// hotel_name -> (time -> (room_count, clients))
	map<string, map<long long int, pair<unsigned int, set<unsigned int>>>> storage;
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
      long long int time;
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
