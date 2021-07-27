#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <algorithm>
#include <utility>
#include <numeric>

#define MAX_CLIENT_ID 10e9

using namespace std;
class BookingSystem {
public:
	BookingSystem() {};

	unsigned int Clients(const string &hotel_name) const
	{
		unsigned int res = 0;
		if (hotel_to_clients.count(hotel_name) != 0){
			auto hotel_data = hotel_to_clients.at(hotel_name);
			auto start_time = current_time - 86400;
			auto start_item = hotel_data.upper_bound(start_time);
			if (start_item != hotel_data.end()){
				res = start_item->second.size();
			}
		}
		return res;
	}

	unsigned int Rooms(const string &hotel_name) const
	{
		unsigned int res = 0;
		if (hotel_to_rooms.count(hotel_name) != 0){
			auto hotel_data = hotel_to_rooms.at(hotel_name);
			auto start_time = current_time - 86400;
			auto start_item = hotel_data.upper_bound(start_time);
			unsigned int rooms = 0;
			if (start_item == hotel_data.begin()){
				rooms = prev(hotel_data.end())->second;
			} else {
				rooms = prev(hotel_data.end())->second - prev(start_item)->second;
			}
			res = rooms;
		}
		return res;
	}

	void Book(const string &hotel_name, long long int time, unsigned int client_id, unsigned int room_count)
	{
		if (hotel_to_rooms.count(hotel_name) != 0 && hotel_to_rooms.at(hotel_name).count(time) == 0){
			unsigned int last_room_count = prev(hotel_to_rooms[hotel_name].end())->second;
			hotel_to_rooms[hotel_name][time] += last_room_count; 
		} 
		current_time = time;
		hotel_to_rooms[hotel_name][time] += room_count;
		hotel_to_clients[hotel_name][time].insert(client_id);

		auto &time_to_clients = hotel_to_clients[hotel_name];
		auto day_ago = time_to_clients.upper_bound(time - 86400);
		for (auto item = day_ago; item != time_to_clients.end(); item = next(item)){
			item->second.insert(client_id);
		}
	}

	void PrintClients(const string &hotel_name)
	{
		auto time_to_clients = hotel_to_clients[hotel_name];
		for (auto item = time_to_clients.begin(); item != time_to_clients.end(); item = next(item)){
			cout << "Stats: Time: " << item->first << " Clients: " << item->second.size() << endl;
		}
	}
private:
	long long int current_time = 0;
	map<string, map<long long int, unsigned int>> hotel_to_rooms;
	map<string, map<long long int, set<unsigned int>>> hotel_to_clients;
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
			//bsystem.PrintClients(hotel_name);
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
