#include "headers/history.h"
bool isPrefix(string text, string pattern) {
	if (pattern.length() > text.length())
		return false;
	for (int i = 0; i < pattern.length(); ++i)
		if (text[i] != pattern[i])
			return false;
	return true;
}

/*
Return 5 most recent query that have "input" as prefix
*/
void logFile(string str, string path) {
	transform(str.begin(), str.end(), str.begin(), ::tolower);
	ofstream fout;
	ifstream fin;
	fin.open(path);
	if (!fin.is_open()) return;
	string temp;
	string output;
	while (getline(fin, temp)) {
		if (temp != str) {
			output += temp + "\n";
		}
	}
	output = str + "\n" + output;
	fin.close();
	fout.open(path);
	fout << output;
	fout.close();
}
vector<string> getRecent(string input, int resultCount, string path) {
	vector<string> res;
	if (input == "")return res;
	ifstream fin;
	transform(input.begin(), input.end(), input.begin(), ::tolower);

	fin.open(path);

	if (!fin.is_open()) {
		res.push_back("Find not found");
		return res;
	}

	while (!fin.eof() && res.size() < resultCount) {
		string line;
		getline(fin, line);
		if (line == "")
			continue;

		if (isPrefix(line, input))
			res.push_back(line);
	}

	fin.close();

	return res;
}

