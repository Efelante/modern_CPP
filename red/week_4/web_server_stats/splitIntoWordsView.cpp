#include "splitIntoWordsView.h"

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
