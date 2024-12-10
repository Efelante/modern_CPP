#pragma once

#include <istream>
#include <ostream>
#include <set>
#include <list>
#include <vector>
#include <map>
#include <string>
using namespace std;

vector<string> SplitIntoWords(const string& line);

class InvertedIndex {
public:
  void Add(string document);
  list<size_t> Lookup(const string& word) const;

  //const string& GetDocument(size_t id) const {
  //  return docs[id];
  //}

private:
  map<string, list<size_t>> index;
  //vector<string> docs;
  size_t docs_count;
};

class SearchServer {
public:
  SearchServer() = default;
  explicit SearchServer(istream& document_input);
  void UpdateDocumentBase(istream& document_input);
  void AddQueriesStream(istream& query_input, ostream& search_results_output);

private:
  InvertedIndex index;
};
