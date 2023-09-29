#include <algorithm> 
#include <cmath> 
#include <iostream> 
#include <list> 
#include <string> 
#include <set> 

#include "test_runner.h"

using namespace std; 

#define MAX_NUMBER 100000

class SportsmenManager 
{ 
public: 
	void AddSportsman(int current, int num) 
	{ 
		list<int>::iterator insert_it;
		if (num_to_queue_pos.find(num) == end(num_to_queue_pos)) {
			insert_it = sportsmen_queue.end();
		} else {
			insert_it = num_to_queue_pos[num];
		}
		num_to_queue_pos[current] = sportsmen_queue.insert(insert_it, current);
	} 
	void PrintQueue() 
	{
		for (auto num: sportsmen_queue) {
			cout << num << " ";
		}
		cout << endl;
	} 
	 
private: 
	list<int> sportsmen_queue;
	map<int, list<int>::iterator> num_to_queue_pos;
};

void TestExample(void)
{
	SportsmenManager sportsmens;
	sportsmens.AddSportsman(2, 5);
	sportsmens.AddSportsman(10, 4);
	sportsmens.AddSportsman(5, 10);
	sportsmens.PrintQueue();
}

int main() 
{
	//TestRunner tr;
	//RUN_TEST(tr, TestExample);

	SportsmenManager sportsmens;
				
	int sportsmen_count; 
	int cur_num; 
	int pos_num; 
	cin >> sportsmen_count; 
	for (int sp = 0; sp < sportsmen_count; ++sp) { 
		cin >> cur_num >> pos_num; 
		sportsmens.AddSportsman(cur_num, pos_num);
	} 
	sportsmens.PrintQueue();
	return 0; 
}
