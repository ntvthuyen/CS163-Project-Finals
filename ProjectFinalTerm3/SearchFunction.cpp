#include "headers/SearchFunction.h"
#include "headers/Misc.h"

bool checkIntitle(string filePath, string key)
{
	ifstream fin;
	fin.open(filePath);

	if (!fin.is_open()) {
		cerr << "Can't open file: " << filePath << endl;
		return false;
	}

	string line;
	while (getline(fin, line)) {
		if (line != "")
			break;
	}

	fin.close();

	vector<string> words = split(line);
	for (int i = 0; i < words.size(); ++i)
		words[i] = simplify(words[i]);

	key = simplify(key);
	for (string word : words)
		if (word == key)
			return true;

	return false;
}

bool checkFileType(string filePath, string key)
{
	int dotPosition = -1;
	for (int i = (int)filePath.length() - 1; i >= 0; --i) {
		if (filePath[i] == '.') {
			dotPosition = i;
			break;
		}
	}

	string extension = filePath.substr(dotPosition + 1);
	return (toLower(extension) == toLower(key));
}

bool checkSynonym(string filePath, Synonym &synonym, string key)
{
	ifstream fin;
	fin.open(filePath);

	if (!fin.is_open()) {
		cerr << "Can't open file: " << filePath << endl;
		return false;
	}

	vector<string> synonymSet = synonym.getSynonym(simplify(key));	
	//for (string word : synonymSet)
	//	cout << word << " ";
	//cout << endl;

	string word;
	while (fin >> word) {
		word = simplify(word);
		if (word == "")
			continue;

		if (binary_search(synonymSet.begin(), synonymSet.end(), word)) {
			fin.close();
			return true;
		}		
	}

	fin.close();
	return false;
}

bool exactMatch(string filePath, string key)
{
	ifstream fin;
	fin.open(filePath);

	if (!fin.is_open()) {
		cerr << "Can't open file: " << filePath << endl;
		return false;
	}

	vector<string> pattern = split(key);
	for (int i = 0; i < pattern.size(); ++i)
		pattern[i] = simplify(pattern[i]);

	string line;
	while (getline(fin, line)) {
		if (line == "")
			continue;

		vector<string> text = split(line);
		for (int i = 0; i < text.size(); ++i)
			text[i] = simplify(text[i]);

		vector<vector<bool> > dp(text.size() + 1, vector<bool>(pattern.size() + 1, false));
		for (int i = 0; i < text.size(); ++i) {
			dp[i][0] = true;
			for (int j = 0; j < pattern.size(); ++j) {
				if (!dp[i][j]) continue;
				if (pattern[j] != "*") {
					if (text[i] == pattern[j]) dp[i + 1][j + 1] = true;
				}
				else {
					for (int k = 1; k <= MAX_ASTERISK_WORD_COUNT; ++k)
						if (i + k <= text.size()) dp[i + k][j + 1] = true;
				}
			}
			if (dp[i + 1][pattern.size()]) {
				fin.close();
				return true;
			}				
		}
	}

	fin.close();
	return false;
}

bool checkHashtag(string filePath, string key)
{
	ifstream fin;
	fin.open(filePath);

	if (!fin.is_open()) {
		cerr << "Can't open file: " << filePath << endl;
		return false;
	}

	//cout << filePath << endl;

	if (key[0] == '#')
		key = key.substr(1);
	key = simplify(key);

	string s;
	while (fin >> s) {
		s = simplify(s);
		if (!s.empty() && s[0] == '#' && s.substr(1) == key) {
			fin.close();
			return true;
		}
	}

	fin.close();
	return false;
}

bool checkNumbersRange(string filePath, double l, double r)
{
	ifstream fin;
	fin.open(filePath);

	if (!fin.is_open()) {
		cerr << "Can't open file: " << filePath << endl;
		return false;
	}

	string s;
	while (fin >> s) {
		if (!s.empty() && '0' <= s[0] && s[0] <= '9') {
			Number num = strToNumber(s);
			if (num.type == NumberType::NORMAL && l <= num.x && num.x <= r) {
				fin.close();
				return true;
			}
		}
	}

	fin.close();
	return false;
}

bool checkPricesRange(string filePath, double l, double r)
{
	ifstream fin;
	fin.open(filePath);

	if (!fin.is_open()) {
		cerr << "Can't open file: " << filePath << endl;
		return false;
	}

	string s;
	while (fin >> s) {
		if (!s.empty() && s[0] == '$') {
			Number num = strToNumber(s);
			if (num.type == NumberType::DOLLAR && l <= num.x && num.x <= r) {
				fin.close();
				return true;
			}
		}
	}

	fin.close();
	return false;
}

const double RELATIVE_ERROR = 0.05;

vector<SearchResult> search(Index &index, Synonym &synonym, const Token &token) {
	vector<SearchResult> cands;
	if (token.op == SearchOperator::PRICE || token.op == SearchOperator::PRICE_RANGE)
		cands = index.getFilesContainWord("$");
	else if (token.op == SearchOperator::HASHTAG)
		cands = index.getFilesContainWord("#");
	else if (token.op == SearchOperator::NUMBER_RANGE) {
		for (int d = 0; d < 10; ++d) {
			string digitStr;
			digitStr += '0' + d;
			cands = SearchResult::Or(cands, index.getFilesContainWord(digitStr));
		}
	}
	else if (token.op == SearchOperator::FILE_TYPE)
		cands = SearchResult::Not(vector<SearchResult>());
	else {
		vector<string> exp = split(token.s);
		bool first = true;
		for (string word: exp) {
			if (word == "*")
				continue;
			vector<SearchResult> wordResults = index.getFilesContainWord(simplify(word));
			if (first)
				cands = wordResults;
			else
				cands = SearchResult::And(cands, wordResults);
		}		
	}

	vector<SearchResult> results;
	for (int i = 0; i < cands.size(); ++i) {
		string filePath = index.getFilePath(cands[i].fileIndex);
		bool found = false;

		switch (token.op) {
		case INTITLE:
			found = checkIntitle(filePath, token.s);
			break;
		case FILE_TYPE:
			found = checkFileType(filePath, token.s);
			break;
		case SYNONYM:
			found = checkSynonym(filePath, synonym, token.s);
			break;
		case HASHTAG:
			found = checkHashtag(filePath, token.s);
			break;
		case SEARCH:
			found = exactMatch(filePath, token.s);
			break;
		case NUMBER_RANGE:
			found = checkNumbersRange(filePath, token.lo.x, token.hi.x);
			break;
		case PRICE_RANGE:
			found = checkPricesRange(filePath, token.lo.x, token.hi.x);
			break;
		case PRICE:
			found = checkPricesRange(filePath, token.lo.x*(1 - RELATIVE_ERROR), token.hi.x*(1 + RELATIVE_ERROR));
			break;
		}

		if (found)
			results.push_back(SearchResult(cands[i].fileIndex));
	}

	return results;
}
