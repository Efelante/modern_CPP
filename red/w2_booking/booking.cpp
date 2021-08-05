#include <iomanip>
#include <iostream>
#include <map>
#include <set>
#include <deque>
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
			long long int start_time = current_time - 86400;
			auto &time = hotel_to_time.at(hotel_name);
			auto &clients = hotel_to_clients.at(hotel_name);
			auto start_item = find_if(time.begin(), time.end(), 
					[start_time](long long int time){return time > start_time;});
			if (start_item != time.end()){
				auto offset = start_item - time.begin();
				set<unsigned int> unique_clients(clients.begin() + offset, clients.end());
				res = unique_clients.size();
				clients.erase(clients.begin(), clients.begin() + offset);
				time.erase(time.begin(), start_item);
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
			if (start_item != hotel_data.end()){
				for (auto item = start_item; item != hotel_data.end(); item = next(item)){
					rooms += item->second;
				}
				res = rooms;
			}
		}
		return res;
	}

	void Book(const string &hotel_name, long long int time, unsigned int client_id, unsigned int room_count)
	{
		current_time = time;
		hotel_to_rooms[hotel_name][time] += room_count;
		hotel_to_time[hotel_name].push_back(time);
		hotel_to_clients[hotel_name].push_back(client_id);
	}

private:
	long long int current_time = 0;
	map<string, map<long long int, unsigned int>> hotel_to_rooms;
	map<string, deque<long long int>> hotel_to_time;
	map<string, deque<unsigned int>> hotel_to_clients;
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
