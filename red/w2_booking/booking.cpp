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
class Hotel {
public:
		Hotel(){};
		void Clear(long long int start_time)
		{
			while (!time.empty() && time.front() <= start_time){
				time.pop();
				rooms_count -= rooms.front();
				rooms.pop();
				clients_count[clients.front()] -= 1;
				clients.pop();
			}
		}

		unsigned int GetClientsCount(){
				return count_if(clients_count.begin(), clients_count.end(), [](const auto &client){return (client.second > 0);});
		}

		unsigned int GetRoomsCount() const{
				return rooms_count; 
		}

		void Book(long long int _time, unsigned int client_id, unsigned int room_count)
	{
		time.push(_time);
		rooms.push(room_count);
		rooms_count += room_count;
		clients.push(client_id);
		clients_count[client_id] += 1;
	}
private:
		unsigned int rooms_count;
		map<unsigned int, unsigned int> clients_count;
		queue<unsigned int> clients;
		queue<unsigned int> rooms;
		queue<long long int> time;
};
class BookingSystem {
public:
	BookingSystem() {};

	unsigned int Clients(const string &hotel_name)
	{
		if (name_to_hotel.count(hotel_name) != 0){
			return name_to_hotel[hotel_name].GetClientsCount();
		} else {
			return 0;
		}
	}

	unsigned int Rooms(const string &hotel_name) const
	{
		if (name_to_hotel.count(hotel_name) != 0){
			return name_to_hotel.at(hotel_name).GetRoomsCount();
		} else {
			return 0;
		}
	}

	void Book(const string &hotel_name, long long int time, unsigned int client_id, unsigned int room_count)
	{
		Hotel &hotel = name_to_hotel[hotel_name];
		hotel.Book(time, client_id, room_count);
		if (time != current_time){
			for (auto &[name, hotelObj]: name_to_hotel){
				hotelObj.Clear(time - 86400);
			}
		}
		current_time = time;
	}

private:
	long long int current_time = 0;
	map<string, Hotel> name_to_hotel;
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
