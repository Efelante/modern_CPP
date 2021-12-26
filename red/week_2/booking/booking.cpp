#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <algorithm>
#include <utility>
#include <numeric>

using namespace std;

struct Event {
		string name;
		long long int time;
		unsigned int client_id;
		unsigned int room_count;
};

class BookingSystem {
public:
	BookingSystem() {};

	unsigned int Clients(const string &hotel_name) const
	{
		if (hotel_clients.count(hotel_name) != 0){
				return hotel_clients.at(hotel_name).size();
		} else {
			return 0;
		}
	}

	unsigned int Rooms(const string &hotel_name) const
	{
		if (hotel_rooms.count(hotel_name) != 0){
			return hotel_rooms.at(hotel_name);
		} else {
			return 0;
		}
	}

	void Book(const string &hotel_name, long long int time, unsigned int client_id, unsigned int room_count)
	{
		events.push({hotel_name, time, client_id, room_count});
		hotel_rooms[hotel_name] += room_count;
		hotel_clients[hotel_name][client_id] += 1;
		while (events.front().time <= (time - 86400)){
				hotel_rooms[events.front().name] -= events.front().room_count;
				hotel_clients[events.front().name][events.front().client_id] -= 1;
				if (hotel_clients[events.front().name][events.front().client_id] == 0){
				hotel_clients[events.front().name].erase(events.front().client_id);
				}
				events.pop();
		}
	}

private:
	map<string, unsigned int> hotel_rooms;
	map<string, map<unsigned int, unsigned int>> hotel_clients;
	queue<Event> events;
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
			string hotel_name;
			unsigned int client_id;
			unsigned int room_count;
			cin >> time >> hotel_name >> client_id >> room_count;

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
