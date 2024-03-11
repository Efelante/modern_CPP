#include "search_server.h"
#include "parse.h"
#include "test_runner.h"

#include <algorithm>
#include <iterator>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <random>
#include <cstdlib>
#include <ctime>
#include <thread>
using namespace std;

void TestFunctionality(
  const vector<string>& docs,
  const vector<string>& queries,
  const vector<string>& expected
) {
  istringstream docs_input(Join('\n', docs));
  istringstream queries_input(Join('\n', queries));

  SearchServer srv;
  srv.UpdateDocumentBase(docs_input);
  ostringstream queries_output;
  srv.AddQueriesStream(queries_input, queries_output);

  const string result = queries_output.str();
  const auto lines = SplitBy(Strip(result), '\n');
  ASSERT_EQUAL(lines.size(), expected.size());
  for (size_t i = 0; i < lines.size(); ++i) {
    ASSERT_EQUAL(lines[i], expected[i]);
  }
}

void TestSerpFormat() {
  const vector<string> docs = {
    "london is the capital of great britain",
    "i am travelling down the river"
  };
  const vector<string> queries = {"london", "the"};
  const vector<string> expected = {
    "london: {docid: 0, hitcount: 1}",
    Join(' ', vector{
      "the:",
      "{docid: 0, hitcount: 1}",
      "{docid: 1, hitcount: 1}"
    })
  };

  TestFunctionality(docs, queries, expected);
}

void TestTop5() {
  const vector<string> docs = {
    "milk a",
    "milk b",
    "milk c",
    "milk d",
    "milk e",
    "milk f",
    "milk g",
    "water a",
    "water b",
    "fire and earth"
  };

  const vector<string> queries = {"milk", "water", "rock"};
  const vector<string> expected = {
    Join(' ', vector{
      "milk:",
      "{docid: 0, hitcount: 1}",
      "{docid: 1, hitcount: 1}",
      "{docid: 2, hitcount: 1}",
      "{docid: 3, hitcount: 1}",
      "{docid: 4, hitcount: 1}"
    }),
    Join(' ', vector{
      "water:",
      "{docid: 7, hitcount: 1}",
      "{docid: 8, hitcount: 1}",
    }),
    "rock:",
  };
  TestFunctionality(docs, queries, expected);
}

void TestHitcount() {
  const vector<string> docs = {
    "the river goes through the entire city there is a house near it",
    "the wall",
    "walle",
    "is is is is",
  };
  const vector<string> queries = {"the", "wall", "all", "is", "the is"};
  const vector<string> expected = {
    Join(' ', vector{
      "the:",
      "{docid: 0, hitcount: 2}",
      "{docid: 1, hitcount: 1}",
    }),
    "wall: {docid: 1, hitcount: 1}",
    "all:",
    Join(' ', vector{
      "is:",
      "{docid: 3, hitcount: 4}",
      "{docid: 0, hitcount: 1}",
    }),
    Join(' ', vector{
      "the is:",
      "{docid: 3, hitcount: 4}",
      "{docid: 0, hitcount: 3}",
      "{docid: 1, hitcount: 1}",
    }),
  };
  TestFunctionality(docs, queries, expected);
}

void TestRanking() {
  const vector<string> docs = {
    "london is the capital of great britain",
    "paris is the capital of france",
    "berlin is the capital of germany",
    "rome is the capital of italy",
    "madrid is the capital of spain",
    "lisboa is the capital of portugal",
    "bern is the capital of switzerland",
    "moscow is the capital of russia",
    "kiev is the capital of ukraine",
    "minsk is the capital of belarus",
    "astana is the capital of kazakhstan",
    "beijing is the capital of china",
    "tokyo is the capital of japan",
    "bangkok is the capital of thailand",
    "welcome to moscow the capital of russia the third rome",
    "amsterdam is the capital of netherlands",
    "helsinki is the capital of finland",
    "oslo is the capital of norway",
    "stockgolm is the capital of sweden",
    "riga is the capital of latvia",
    "tallin is the capital of estonia",
    "warsaw is the capital of poland",
  };

  const vector<string> queries = {"moscow is the capital of russia"};
  const vector<string> expected = {
    Join(' ', vector{
      "moscow is the capital of russia:",
      "{docid: 7, hitcount: 6}",
      "{docid: 14, hitcount: 6}",
      "{docid: 0, hitcount: 4}",
      "{docid: 1, hitcount: 4}",
      "{docid: 2, hitcount: 4}",
    })
  };
  TestFunctionality(docs, queries, expected);
}

void TestBasicSearch() {
  const vector<string> docs = {
    "we are ready to go",
    "come on everybody shake you hands",
    "i love this game",
    "just like exception safety is not about writing try catch everywhere in your code move semantics are not about typing double ampersand everywhere in your code",
    "daddy daddy daddy dad dad dad",
    "tell me the meaning of being lonely",
    "just keep track of it",
    "how hard could it be",
    "it is going to be legen wait for it dary legendary",
    "we dont need no education"
  };

  const vector<string> queries = {
    "we need some help",
    "it",
    "i love this game",
    "tell me why",
    "dislike",
    "about"
  };

  const vector<string> expected = {
    Join(' ', vector{
      "we need some help:",
      "{docid: 9, hitcount: 2}",
      "{docid: 0, hitcount: 1}"
    }),
    Join(' ', vector{
      "it:",
      "{docid: 8, hitcount: 2}",
      "{docid: 6, hitcount: 1}",
      "{docid: 7, hitcount: 1}",
    }),
    "i love this game: {docid: 2, hitcount: 4}",
    "tell me why: {docid: 5, hitcount: 2}",
    "dislike:",
    "about: {docid: 3, hitcount: 2}",
  };
  TestFunctionality(docs, queries, expected);
}
/*
 *
 * document_input содержит не более 50000 документов
 *
 * каждый документ содержит не более 1000 слов
 *
 * общее число различных слов во всех документах не
 * превосходит 10000
 *
 * максимальная длина одного слова — 100 символов,
 * слова состоят из строчных латинских букв и
 * разделены одним или несколькими пробелами
 *
 * query_input содержит не более 500 000 запросов,
 * каждый запрос содержит от 1 до 10 слов.
 * */

// Функция формирует слово по числовому шаблону, заменяя каждую цифру на букву.
// Вход - 5-значное десятичное число
// Выход - слово из 100 символов
//
// Из 5-буквенного слова можно сгенерировать 10^5 уникальных слов. Повторив 5-буквенное слово 20 раз получим 100-буквенное уникальное слово.
string GenerateWord(int num_template)
{
	string tmplt(5, 'a');
	string word(100, 'a');
	size_t i = 0;
	while (num_template != 0){
		char letter = 'a' + num_template % 10;
		tmplt[i] = letter;
		++i;
		num_template /= 10;
	}
	//cout << tmplt << endl;
	for (uint8_t offset = 0; offset < 100; offset += 5) {
		tmplt.copy(&word[offset], tmplt.size());
	}
	return word;
}

string GenerateDocument(const vector<string> &dict)
{
	string document;

	srand(time(nullptr));
	for (size_t i = 0; i < 1000; ++i) {
		int doc_num = rand() % 10000;
		document.append(" ");
		document.append(dict[doc_num]);
	}
	return document;
}

string GenerateQuery(const vector<string> &dict)
{
	string query;

	srand(time(nullptr));
	for (size_t i = 0; i < 10; ++i) {
		int query_num = rand() % 10000;
		query.append(" ");
		query.append(dict[query_num]);
	}
	return query;
}

//void TestSearchServer(istream& document_input, istream& query_input,
void TestSearchServer()
{

	const int WORD_MAX_LENGTH = 100;
	const int DIFFERENT_WORDS_COUNT = 10000;
	//const int WORDS_IN_DOCUMENT = 1000;
	const int WORDS_IN_DOCUMENT = 100;
	//const int DOCUMENTS_COUNT = 50000;
	const int DOCUMENTS_COUNT = 10000;
	//const int QUERIES_COUNT = 500000;
	const int QUERIES_COUNT = 20000;
	const int WORDS_IN_QUERY = 10;

	vector<string> dict(DIFFERENT_WORDS_COUNT);

	for (int i = 10000; i < 20000; ++i){
		dict[i - DIFFERENT_WORDS_COUNT] = GenerateWord(i);
		
	}
	//for (int i = 0; i < 10000; ++i){
	//	cout << dict[i] << endl;
	//}
	
	vector<string> documents;
	for (int i = 0; i < DOCUMENTS_COUNT; ++i) {
		documents.push_back(GenerateDocument(dict));
	}

	vector<string> queries;
	for (int i = 0; i < QUERIES_COUNT; ++i) {
		queries.push_back(GenerateQuery(dict));
	}

	//for (const string &doc: documents) {
	//	cout << doc;
	//	cout << endl;
	//}

	istringstream document_input(Join('\n', documents));
	istringstream query_input(Join('\n', queries));
	ostringstream search_results_output;

	SearchServer srv(document_input);
	srv.AddQueriesStream(query_input, search_results_output);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestSerpFormat);
  RUN_TEST(tr, TestTop5);
  RUN_TEST(tr, TestHitcount);
  RUN_TEST(tr, TestRanking);
  RUN_TEST(tr, TestBasicSearch);
  RUN_TEST(tr, TestSearchServer);
}
