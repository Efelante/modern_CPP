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

vector<string_view> SplitIntoWordsView(string_view str)
{

	// Trim whitespaces from the left
	size_t pos = 0;
	while (isspace(str[pos])){
		pos++;
	}
	str.remove_prefix(pos);

	vector<string_view> result;

	while(1){
		size_t space = str.find(" ");
		result.push_back(str.substr(0, space));

		if (space == str.npos){
			break;
		} else {
			str.remove_prefix(space + 1);
		}
	}
	return result;
}

Stats ExplorePub(const set<string> &words, const string &pub){
	Stats stats;
	vector<string_view> pub_words = SplitIntoWordsView(pub);
	for (const auto &pub_word: pub_words){
		if (words.count(string(pub_word))){
			stats.word_frequences[string(pub_word)] += 1;
		}
	}
	return stats;
}

Stats ProcessPubs(const set<string> &key_words, const vector<string> &pubs)
{
	Stats res;
	vector<future<Stats>> fstats;
	for (const auto &pub: pubs){
		fstats.push_back(async(ExplorePub, ref(key_words), ref(pub)));
	}
	for (auto &fstat: fstats){
		res += fstat.get();
	}
	return res;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
	const int pub_limit = 4;
	Stats stats;
	vector<string> pubs;
	string pub;
	while (getline(input, pub)){
		pubs.push_back(pub);
		if (pubs.size() == pub_limit){
			// Process pub_limit pubs
			stats += ProcessPubs(key_words, pubs);
			pubs.clear();
		} else {
			// Keep collecting pubs
		}
	}
	if (pubs.size()){
		// Process the last pubs
		stats += ProcessPubs(key_words, pubs);
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
