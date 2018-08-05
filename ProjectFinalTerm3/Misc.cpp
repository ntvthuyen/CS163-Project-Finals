#include "headers/Misc.h"

vector<string> split(string s, char delimeter) {
	vector<string> results;
	string curWord;

	for (char ch : s) {
		if (ch == delimeter) {
			if (curWord != "")
				results.push_back(curWord);
			curWord = "";
		}
		else
			curWord += ch;
	}
	if (curWord != "")
		results.push_back(curWord);

	return results;
}

//bool isRegular(char c) {
//	if (c < 0 || c > 255) 
//		return false;
//	return (isalnum(tolower(c)) || c == '#' || c == '$');
//}

string simplify(string word)
{
	if (word == "*")
		return word;

	string res;
	for (char c : word) {
		if (c < 0 || c > 255) continue;
		bool toAdd = false;
		if (res.empty())
			toAdd = (isalnum(tolower(c)) || c == '#' || c == '$');
		else
			toAdd = true;

		if (toAdd)
			res += tolower(c);
	}

	while (!res.empty() && !isalnum(res.back()))
		res.pop_back();
	
	return res;
}

string toLower(string s) {
	string res;
	for (int i = 0; i < s.length(); ++i)
		res += tolower(s[i]);
	return res;
}
