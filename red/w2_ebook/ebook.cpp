#include <iomanip>
#include <iostream>
#include <map>
#include <algorithm>
#include <numeric>

using namespace std;

class ReadingManager {
public:
	ReadingManager() {};

	void Read(int user_id, int page_count) {
		if (user_page_counts.count(user_id) != 0){
			int old_count = user_page_counts[user_id];
			page_counts_user_counts[old_count] -= 1;
		}
		user_page_counts[user_id] = page_count;
		page_counts_user_counts[page_count] += 1;
	}

	double Cheer(int user_id) const {
		if (user_page_counts.count(user_id) == 0) {
			return 0;
		}
		const int user_count = GetUserCount();
		if (user_count == 1) {
			return 1;
		}

		int page_count = user_page_counts.at(user_id);
		auto lbound = page_counts_user_counts.lower_bound(page_count);

		auto part = accumulate(page_counts_user_counts.begin(),
				lbound,
				0,
				[](int &sum, const pair<int, int> &item){
				return sum + item.second;
				}
				);
		return part * 1.0 / (user_count - 1);
}

private:
	map<int, int> page_counts_user_counts;
	map<int, int> user_page_counts;

	int GetUserCount() const {
		return user_page_counts.size();
	}
};


int main() {
	// Для ускорения чтения данных отключается синхронизация
	// cin и cout с stdio,
	// а также выполняется отвязка cin от cout
	ios::sync_with_stdio(false);
	cin.tie(nullptr);

	ReadingManager manager;

	int query_count;
	cin >> query_count;

	for (int query_id = 0; query_id < query_count; ++query_id) {
		string query_type;
		cin >> query_type;
		int user_id;
		cin >> user_id;

		if (query_type == "READ") {
			int page_count;
			cin >> page_count;
			manager.Read(user_id, page_count);
		} else if (query_type == "CHEER") {
			cout << setprecision(6) << manager.Cheer(user_id) << "\n";
		}
	}

	return 0;
}
