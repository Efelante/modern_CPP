#include "stats.h"
#include "splitIntoWordsView.h"

#include <vector>
#include <iostream>

void Stats::AddMethod(string_view method)
{
	method = (method_count.count(method) == 0)? "UNKNOWN": method;
	method_count[method]++;
}

void Stats::AddUri(string_view uri)
{
	uri = (url_count.count(uri) == 0)? "unknown": uri;
	url_count[uri]++;
}

const map<string_view, int>& Stats::GetMethodStats() const
{
	return method_count;
}

const map<string_view, int>& Stats::GetUriStats() const
{
	return url_count; 
}

HttpRequest ParseRequest(string_view line)
{
	vector<string_view> line_words = SplitIntoWordsView(line); 
	string_view method 	 = line_words[0];
	string_view uri 	 = line_words[1];
	string_view protocol = line_words[2];
	return {method, uri, protocol};
}
