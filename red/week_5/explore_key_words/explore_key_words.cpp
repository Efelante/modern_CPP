#include "test_runner.h"
#include "profile.h"

#include <map>
#include <string>
#include <future>
#include <functional>
#include <numeric>
#include <iostream>
#include <utility>
#include <iterator>

using namespace std;

template <typename Iterator>
class Page {
public:
	Page(Iterator begin, Iterator end)
	: _begin(begin), _end(end)
	{
		// Empty body
	}

	Iterator begin()
	{
		return _begin;
	}

	Iterator end()
	{
		return _end;
	}

	Iterator begin() const
	{
		return _begin;
	}

	Iterator end() const
	{
		return _end;
	}


	size_t size() const
	{
		return (_end - _begin);
	}
private:
	Iterator _begin;
	Iterator _end;
};

// Реализуйте шаблон класса Paginator

template <typename Iterator>
class Paginator {
public:
	Paginator(Iterator begin, Iterator end, size_t page_size)
	{
		Iterator page_start = begin;
		Iterator page_end;
		auto container_size = distance(begin, end);
		size_t full_count = container_size / page_size;
		for (size_t i = 0; i < full_count; ++i){
			page_end = next(page_start, page_size);
			pages.push_back(Page(page_start, page_end));
			page_start = page_end;
		}
		if (container_size % page_size){
			pages.push_back(Page(page_start, end));
		}
	}

	size_t size() const
	{
		return pages.size();
	}

	auto begin()
	{
		return pages.begin();
	}
	
	auto end()
	{
		return pages.end();
	}
	// Why don't I need const begin and end
	// Because of auto?
private:
	vector<Page<Iterator>> pages;
};

template <typename C>
auto Paginate(C& c, size_t page_size) {
  return Paginator{c.begin(), c.end(), page_size};
}


struct Stats {
  map<string, int> word_frequences;

  void operator += (const Stats& other);
};

void Stats::operator += (const Stats& other)
{
	for (const auto &[word, cnt]: other.word_frequences){
		word_frequences[word] += cnt;
	}
}

Stats ExploreKeyword(const string &word, const string &pub){
	Stats stats;
	size_t word_size = word.size();
	size_t match_pos = pub.find(word);
	while(match_pos != string::npos){
		if (0
			|| ((match_pos == 0) && ((match_pos + word_size) == pub.size())) 						
			|| ((match_pos == 0) && (pub[match_pos + word_size] ==  ' ')) 					 
			|| ((pub[match_pos - 1] == ' ') && (pub[match_pos + word_size] == ' ')) 	 
			|| ((pub[match_pos - 1] == ' ') && ((match_pos + word_size) == pub.size()))
		   ){
			stats.word_frequences[word] += 1;
		}
		match_pos = pub.find(word, match_pos + word_size);
	}
	return stats;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
  const int page_size = 4;
  Stats stats;
  string pub;
  while (getline(input, pub)){
  	  vector<future<Stats>> word_fut_stat;
	  for (auto &page: Paginate(key_words, page_size)){
		  word_fut_stat.push_back(
	          async(
		          [page, &pub](){
			      	Stats stat;
			      	for (const auto &word: page){
			      	    stat += ExploreKeyword(word, pub);
			      	}
			      	return stat;
			      }
			  )
		  );

	  }
	  for (future<Stats> &f: word_fut_stat){
	     stats += f.get();
	  }
  }
  return stats;
}

void TestBasic() {
  const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

  stringstream ss;
  ss << "this new yangle service really rocks\n";
  ss << "It sucks when yangle isn't available\n";
  ss << "10 reasons why yangle is the best IT company\n";
  ss << "yangle rocks others suck\n";
  ss << "Goondex really sucks, but yangle rocks. Use yangle\n";

  const auto stats = ExploreKeyWords(key_words, ss);
  const map<string, int> expected = {
    {"yangle", 6},
    {"rocks", 2},
    {"sucks", 1}
  };
  ASSERT_EQUAL(stats.word_frequences, expected);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestBasic);
}
