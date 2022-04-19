#include "test_runner.h"
#include "profile.h"

#include <map>
#include <string>
using namespace std;

struct Stats {
  map<string, int> word_frequences;

  void operator += (const Stats& other);
};

Stats ExploreKeyWords(const set<string>& key_words, istream& input) {
  Stats stats;
  string pub;
  while (getline(input, pub)){
	  for (const string &word: key_words){
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
