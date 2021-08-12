#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <algorithm>
#include <utility>
#include <numeric>

#define MAX_CLIENT_ID 10e9

using namespace std;
class BookingSystem {
public:
	BookingSystem() {};

	unsigned int Clients(const string &hotel_name)
	{
		unsigned int res = 0;
		if (hotel_to_clients.count(hotel_name) != 0){
			auto clients = hotel_to_clients.at(hotel_name);
			auto offset = hotel_to_offset.at(hotel_name);
			set<unsigned int> unique_clients(clients.begin() + offset, clients.end());
			res = unique_clients.size();
		}
		return res;
	}

	unsigned int Rooms(const string &hotel_name) const
	{
		unsigned int res = 0;
		if (hotel_to_rooms.count(hotel_name) != 0){
			auto rooms = hotel_to_rooms.at(hotel_name);
			auto offset = hotel_to_offset.at(hotel_name);
			unsigned int rooms_count = accumulate(rooms.begin() + offset, rooms.end(), 0);
			res = rooms_count;
		}
		return res;
	}

	void Book(const string &hotel_name, long long int time, unsigned int client_id, unsigned int room_count)
	{
		hotel_to_rooms[hotel_name].push_back(room_count);
		hotel_to_time[hotel_name].push_back(time);
		hotel_to_clients[hotel_name].push_back(client_id);
		int offset = hotel_to_offset[hotel_name];
		long long int start_time = hotel_to_time[hotel_name][offset];
		if ((time != current_time) && (start_time <= (time - 86400))){
			current_time = time;
			for (auto &[hotel, time]: hotel_to_time){
				ClearData(hotel);
			}
		}
		current_time = time;
	}

private:
	void ClearData (const string &hotel_name)
	{
		long long int start_time = current_time - 86400;
		auto &time = hotel_to_time.at(hotel_name);
		auto &offset = hotel_to_offset[hotel_name];
		auto start_item = upper_bound(time.begin() + offset, time.end(), start_time);
		offset = start_item - time.begin();

	}
	long long int current_time = 0;
	map<string, int> hotel_to_offset;
	map<string, vector<unsigned int>> hotel_to_rooms;
	map<string, vector<long long int>> hotel_to_time;
	map<string, vector<unsigned int>> hotel_to_clients;
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
