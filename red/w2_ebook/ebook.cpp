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
		if (user_page_counts.count(user_id) != 0){		// Log Q
			int old_count = user_page_counts[user_id];	// Log Q
			page_counts_user_counts[old_count] -= 1;	// Log Q
		}
		user_page_counts[user_id] = page_count;			// Log Q
		page_counts_user_counts[page_count] += 1;		// Log Q
	}

	double Cheer(int user_id) const {
		if (user_page_counts.count(user_id) == 0) {		// Log Q
			return 0;
		}
		const int user_count = GetUserCount();			// Constant
		if (user_count == 1) {
			return 1;
		}

		int page_count = user_page_counts.at(user_id);		// Log Q
		int sum = 0;
		// Accumulate has linear complexity of the distance between first and last
		for (auto it = page_counts_user_counts.begin(); it->first != page_count; it = next(it)){	// Q (?)
			sum += it->second;	
		}
		return sum * 1.0 / (user_count - 1);
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
