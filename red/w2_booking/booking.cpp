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
		if (hotel_to_client_counts.count(hotel_name) != 0){
			auto &client_to_count = hotel_to_client_counts.at(hotel_name);
			res = count_if(client_to_count.begin(), client_to_count.end(), [](const auto &client){return (client.second > 0);});
		}
		return res;
	}

	unsigned int Rooms(const string &hotel_name) const
	{
		if (hotel_to_rooms_count.count(hotel_name) != 0){
			return hotel_to_rooms_count.at(hotel_name);
		} else {
			return 0;
		}
	}

	void Book(const string &hotel_name, long long int time, unsigned int client_id, unsigned int room_count)
	{
		hotel_to_time[hotel_name].push(time);

		hotel_to_rooms[hotel_name].push(room_count);
		hotel_to_rooms_count[hotel_name] += room_count;

		hotel_to_clients[hotel_name].push(client_id);
		hotel_to_client_counts[hotel_name][client_id] += 1;

		if (time != current_time){
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
		auto &rooms = hotel_to_rooms.at(hotel_name);
		auto &rooms_count = hotel_to_rooms_count.at(hotel_name);
		auto &clients = hotel_to_clients.at(hotel_name);
		auto &clients_count = hotel_to_client_counts.at(hotel_name);
		while (!time.empty() && time.front() <= start_time){
			time.pop();
			rooms_count -= rooms.front();
			rooms.pop();
			clients_count[clients.front()] -= 1;
			clients.pop();
		}
	}
	long long int current_time = 0;
	map<string, queue<long long int>> hotel_to_time;
	map<string, queue<unsigned int>> hotel_to_rooms;
	map<string, unsigned int> hotel_to_rooms_count;
	map<string, queue<unsigned int>> hotel_to_clients;
	map<string, map<unsigned int, unsigned int>> hotel_to_client_counts;
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
