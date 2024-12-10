#include "splitIntoWordsView.h"
#include <iostream>
#include <stdio.h>

vector<string_view> SplitIntoWordsView(string_view str)
{

	vector<string_view> result;

	while(1){
		// Trim whitespaces from the left
		size_t pos = 0;
		while ((str[pos] != 0) && 
			isspace(str[pos])){
			//cout << "|" << str[pos] << "| is space" << endl;
			pos++;
		}
		if (0 == str[pos]) {
			break;
		} else {
			str.remove_prefix(pos);
			size_t space = str.find(" ");
			result.push_back(str.substr(0, space));
			if (space == str.npos){
				break;
			} else {
				str.remove_prefix(space);
			}
		}
	}
	return result;
}
