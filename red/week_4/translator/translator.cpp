#include "test_runner.h"
#include <string>
#include <vector>

using namespace std;

class Translator {
public:
  void Add(string_view source, string_view target);
  string_view TranslateForward(string_view source) const;
  string_view TranslateBackward(string_view target) const;

private:
  set<string> data;
  map<string_view, string_view> forward;
  map<string_view, string_view> backward;
};

void Translator::Add(string_view source, string_view target)
{
	auto sres = data.insert(string(source));
	auto tres = data.insert(string(target));
	forward[*sres.first] = *tres.first;
	backward[*tres.first] = *sres.first;
}

string_view Translator::TranslateForward(string_view source) const
{
	auto sit = data.find(string(source));
	if ((sit != data.end()) && forward.count(*sit)){
		return forward.at(*sit);
	} else {
		return string("");
	}
}

string_view Translator::TranslateBackward(string_view target) const
{
	auto tit = data.find(string(target));
	if ((tit != data.end()) && backward.count(*tit)){
		return backward.at(*tit);
	} else {
		return string("");
	}
}

void TestSimple() {
  Translator translator;
  translator.Add(string("okno"), string("window"));
  translator.Add(string("stol"), string("table"));

  ASSERT_EQUAL(translator.TranslateForward("okno"), "window");
  ASSERT_EQUAL(translator.TranslateBackward("table"), "stol");
  ASSERT_EQUAL(translator.TranslateBackward("stol"), "");
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSimple);
  return 0;
}
