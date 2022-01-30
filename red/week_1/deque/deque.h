#include <vector>
#include <stdexcept>

using namespace std;

template<typename T>
class Deque {
public:
	Deque(){}

	bool Empty() const
	{
		return !Size();
	}

	size_t Size() const
	{
		return (front.size() + back.size());
	}

	T& operator[](size_t index)
	{
		size_t fsize = front.size();
		if (index < fsize){
			return front[fsize - 1 - index];	
		} else {
			return back[index - fsize];
		}
	}

	const T& operator[](size_t index) const
	{
		size_t fsize = front.size();
		if (index < fsize){
			return front[fsize - 1 - index];	
		} else {
			return back[index - fsize];
		}
	}

	void PushFront(const T& item)
	{
		front.push_back(item);	
	}

	void PushBack(const T& item)
	{
		back.push_back(item);	
	}

	T& At(size_t index)
	{
		if ((index + 1) > Size()){
			throw out_of_range("Index is out of range");
		} else {
			size_t fsize = front.size();
			if (index < fsize){
				return front[fsize - 1 - index];	
			} else {
				return back[index - fsize];
			}
		}
	}

	const T& At(size_t index) const
	{
		if ((index + 1) > Size()){
			throw out_of_range("Index is out of range");
		} else {
			size_t fsize = front.size();
			if (index < fsize){
				return front[fsize - 1 - index];	
			} else {
				return back[index - fsize];
			}
		}
	}

	T& Front()
	{
		if (0 != front.size()){
			return front.back();
		} else {
			return back[0];
		}
	}

	const T& Front() const
	{
		if (0 != front.size()){
			return front.back();
		} else {
			return back[0];
		}
	}

	T& Back()
	{
		if (0 != back.size()){
			return back.back();
		} else {
			return front[0];
		}
	}

	const T& Back() const
	{
		if (0 != back.size()){
			return back.back();
		} else {
			return front[0];
		}
	}

private:
	vector<T> front;
	vector<T> back;
};
