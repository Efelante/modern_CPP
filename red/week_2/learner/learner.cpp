#include <algorithm>
#include <string>
#include <vector>
#include <set>

#include "profile.h" 

using namespace std;

class Learner {
 private:
  set<string> dict;

 public:
  int Learn(const vector<string>& words) {
	  LOG_DURATION("Learn");
    auto cur_size = dict.size();
    dict.insert(words.begin(), words.end());
    return (dict.size() - cur_size);
  }

  vector<string> KnownWords() {
	  LOG_DURATION("Generate known words");
    return {dict.begin(), dict.end()};
  }
};
