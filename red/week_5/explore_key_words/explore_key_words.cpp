#include "test_runner.h"
#include "profile.h"

#include <map>
#include <set>
#include <string>
#include <future>

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

Stats ExploreKeyWordsSingleThread(const set<string>& key_words, istream& input) {
  Stats stats;
  string pub;
  while (input >> pub){
	  if (key_words.count(pub)){
		  stats.word_frequences[pub] += 1;
	  }
  }
  return stats;
}

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
	const int threads_num = 8;
	Stats stats;
	vector<future<Stats>> fstats;
	vector<stringstream> paper_streams(threads_num);
	string pub;
	int i = 0;
	while (getline(input, pub)){
		paper_streams[i] << pub << "\n";
		i = (i + 1) % threads_num;
	}
	for (auto &ss: paper_streams){
		fstats.push_back(async(ExploreKeyWordsSingleThread, ref(key_words), ref(ss)));
	}
	for (auto &fs: fstats){
		stats += fs.get();
	}
	return stats;
}

void TestBasic() {
	const int pnum = 100000;

	const set<string> key_words = {"yangle", "rocks", "sucks", "all"};

	stringstream ss;
	for (int i = 0; i < pnum; ++i){
		ss << "this new yangle service really rocks\n";
		ss << "It sucks when yangle isn't available\n";
		ss << "10 reasons why yangle is the best IT company\n";
		ss << "yangle rocks others suck\n";
		ss << "Goondex really sucks, but yangle rocks. Use yangle\n";
	}

	{LogDuration("Single thread");

		const auto stats = ExploreKeyWords(key_words, ss);
		const map<string, int> expected = {
			{"yangle", pnum * 6},
			{"rocks",  pnum * 2},
			{"sucks",  pnum * 1}
		};
		for (const auto &[key, val]: stats.word_frequences){
			cout << key << " " << val << endl;
		}
		ASSERT_EQUAL(stats.word_frequences, expected);
	}
}

int main() {
	TestRunner tr;
	RUN_TEST(tr, TestBasic);
}
