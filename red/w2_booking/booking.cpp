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
		auto hotel_data = storage.at(hotel_name);
		set<unsigned int> clients;
		for (const auto &item: hotel_data){
			clients.insert(item.second.second.begin(), item.second.second.end());
		}
		return clients.size();
	}
	unsigned int Rooms(const string &hotel_name) const
	{
		auto hotel_data = storage.at(hotel_name);
		unsigned int rooms = 0;
		for (const auto &item: hotel_data){
			rooms += item.second.first;
		}
		return rooms;
	}
    void Book(const string &hotel_name, int time, unsigned int client_id, unsigned int room_count)
	{
		storage[hotel_name][time].first += room_count;
		storage[hotel_name][time].second.insert(client_id); 
	}
private:
	// hotel_name -> (time -> (room_count, clients))
	map<string, map<int, pair<unsigned int, set<unsigned int>>>> storage;
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
