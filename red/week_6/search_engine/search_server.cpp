#include "search_server.h"
#include "iterator_range.h"
#include "splitIntoWordsView.h"
#include "profile.h"

#include <algorithm>
#include <iterator>
#include <sstream>
#include <iostream>

vector<string> SplitIntoWords(const string& line) {
	LOG_DURATION("F: SplitIntoWords");
	istringstream words_input(line);
	return {istream_iterator<string>(words_input), istream_iterator<string>()};
}

SearchServer::SearchServer(istream& document_input) {
  UpdateDocumentBase(document_input);
}

void SearchServer::UpdateDocumentBase(istream& document_input) {
	LOG_DURATION("F: UpdateDocumentBase")

	InvertedIndex new_index;

	{LOG_DURATION("new_index.Add");
	for (string current_document; getline(document_input, current_document); ) {
		new_index.Add(std::move(current_document));
	}
	}


	{LOG_DURATION("move new_index");
	index = std::move(new_index);
	}
}

void SearchServer::AddQueriesStream(
  istream& query_input, ostream& search_results_output
) {
	LOG_DURATION("AddQueriesStream");
  for (string current_query; getline(query_input, current_query); ) {
    const auto words = SplitIntoWordsView(current_query);

    // Отображение docid -> кол-во повторов всех
    // 			    слов запроса в документе
    map<size_t, size_t> docid_count;
    {LOG_DURATION("Lookup");
	    for (const auto& word : words) {
		    for (const size_t docid : index.Lookup(string(word))) {
			    docid_count[docid]++;
		    }
	    }
    }
    {LOG_DURATION("Create vector of pairs from map");
	    vector<pair<size_t, size_t>> search_results(
			    docid_count.begin(), docid_count.end()
			    );
    }
    vector<pair<size_t, size_t>> search_results(
      docid_count.begin(), docid_count.end()
    );
    // Сортируем {docid, count} по убыванию
    // При совпадении значения метрики выбирается
    // более ранний документ (с меньшим id)
    {LOG_DURATION("Sort results");
    sort(
      begin(search_results),
      end(search_results),
      [](pair<size_t, size_t> lhs, pair<size_t, size_t> rhs) {
        int64_t lhs_docid = lhs.first;
        auto lhs_hit_count = lhs.second;
        int64_t rhs_docid = rhs.first;
        auto rhs_hit_count = rhs.second;
        return make_pair(lhs_hit_count, -lhs_docid) > make_pair(rhs_hit_count, -rhs_docid);
      }
    );
    }

    {LOG_DURATION("Gen search results");
    search_results_output << current_query << ':';
    for (auto [docid, hitcount] : Head(search_results, 5)) {
      search_results_output << " {"
        << "docid: " << docid << ", "
        << "hitcount: " << hitcount << '}';
    }
    search_results_output << endl;
    }
  }
}

void InvertedIndex::Add(string document) {
	//{LOG_DURATION("F: Add.docs.push_back");
	//docs.push_back(std::move(document));
	//}

	//const size_t docid = docs.size() - 1;
	const size_t docid = docs_count++;
	//{LOG_DURATION("F: Add.index.push_back");
	//for (const auto& word : SplitIntoWordsView(docs[docid])) {
	for (const auto& word : SplitIntoWordsView(document)) {
		index[string(word)].push_back(docid);
	}
	//}
}

list<size_t> InvertedIndex::Lookup(const string& word) const {
  if (auto it = index.find(word); it != index.end()) {
    return it->second;
  } else {
    return {};
  }
}
