#include "deque.h"
#include <iostream>

using namespace std;

int main(void)
{
	Deque<int> d;

	d.PushFront(5);
	d.PushFront(6);
	d.PushBack(3);
	d.PushBack(2);

	cout << d[0] << endl;
	d[0] = 7;
	cout << d[0] << endl;
	d.At(0) = 8;
	cout << d[0] << endl;

	for (int i = 0; i < d.Size(); ++i){
		cout << d[i] << " ";
	}
	cout << endl;

	cout << d.Size() << endl;
	d.At(3) = 8;

	return 0;
}
