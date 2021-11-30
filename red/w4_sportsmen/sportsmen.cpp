#include <algorithm> 
#include <cmath> 
#include <iostream> 
#include <map> 
#include <string> 
#include <set> 

#include "test_runner.h"

using namespace std; 

class RouteManager 
{ 
public: 
	void AddRoute(int start, int finish) 
	{ 
		reachable_lists_[start].insert(finish);
		reachable_lists_[finish].insert(start); 
	} 
	int FindNearestFinish(int start, int finish) const 
	{ 
		int result = abs(start - finish); 
		// If there is not express with a start station
		if (reachable_lists_.count(start) < 1) { 
			return result; 
		}
		const set<int>& reachable_stations = reachable_lists_.at(start); 
		if (!reachable_stations.empty()) { 
			// If there is direct route
			if (reachable_stations.count(finish)){
				result = 0;
			} else {
				// Find nearest right station
				auto nr = reachable_stations.upper_bound(finish);
				if (nr == reachable_stations.begin() && 
					nr != reachable_stations.end()){
					result = min(result, abs(finish - *nr));
				}
				if (nr != reachable_stations.begin() && 
					nr != reachable_stations.end()){
					// find nearest left station
					auto nl = prev(nr);
					result = min(result, abs(finish - *nr));
					result = min(result, abs(finish - *nl));
				}
				if (nr != reachable_stations.begin() && 
					nr == reachable_stations.end()){
					// find nearest left station
					auto nl = prev(nr);
					result = min(result, abs(finish - *nl));
				}
			}
		} 
		return result; 
	} 
private: 
	map<int, set<int>> reachable_lists_; 
};

void TestExample(void)
{
	RouteManager routes;
	routes.AddRoute(-2, 5);
	routes.AddRoute(10, 4);
	routes.AddRoute(5, 8);
	ASSERT_EQUAL(routes.FindNearestFinish(4, 10), 0);
	ASSERT_EQUAL(routes.FindNearestFinish(4, -2), 6);
	ASSERT_EQUAL(routes.FindNearestFinish(5, 0), 2);
	ASSERT_EQUAL(routes.FindNearestFinish(5, 100), 92);
}

int main() 
{
	//TestRunner tr;
	//RUN_TEST(tr, TestExample);
				
	RouteManager routes; 
	int query_count; 
	cin >> query_count; 
	for (int query_id = 0; query_id < query_count; ++query_id) { 
		string query_type; 
		cin >> query_type; 
		int start, finish; 
		cin >> start >> finish; 
		if (query_type == "ADD") { 
			routes.AddRoute(start, finish); 
		} else if (query_type == "GO") { 
			cout << routes.FindNearestFinish(start, finish) << "\n"; 
		} 
	} 
	return 0; 
}
