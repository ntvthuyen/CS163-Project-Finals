#include "headers/RankFunction.h"

double scoreIntitle(string filePath, string key)
{
	ifstream fin;
	fin.open(filePath);

	if (!fin.is_open()) {
		cerr << "Can't open file: " << filePath << endl;
		return 0.0;
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
	double score = 0;
	for (string word : words)
		if (word == key)
			score += 1;

	return score;
}

double scoreFileType(string filePath, string key)
{
	return 0.0;
}

double scoreSynonym(string filePath, Synonym & synonym, string key)
{
	ifstream fin;
	fin.open(filePath);

	if (!fin.is_open()) {
		cerr << "Can't open file: " << filePath << endl;
		return 0.0;
	}

	vector<string> synonymSet = synonym.getSynonym(simplify(key));
	//for (string word : synonymSet)
	//	cout << word << " ";
	//cout << endl;

	string word;

	double score = 0;
	while (fin >> word) {
		word = simplify(word);
		if (word == "")
			continue;

		if (binary_search(synonymSet.begin(), synonymSet.end(), word))
			++score;
	}

	fin.close();
	return score;

}

double scoreMatch(string filePath, string key)
{
	ifstream fin;
	fin.open(filePath);

	if (!fin.is_open()) {
		cerr << "Can't open file: " << filePath << endl;
		return 0.0;
	}

	vector<string> pattern = split(key);
	for (int i = 0; i < pattern.size(); ++i)
		pattern[i] = simplify(pattern[i]);

	string line;
	double score = 0;
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
			if (dp[i + 1][pattern.size()])
				score += 1;
		}
	}

	fin.close();
	return score;
}

double scoreHashtag(string filePath, string key)
{
	ifstream fin;
	fin.open(filePath);

	if (!fin.is_open()) {
		cerr << "Can't open file: " << filePath << endl;
		return 0.0;
	}

	//cout << filePath << endl;

	if (key[0] == '#')
		key = key.substr(1);
	key = simplify(key);

	string s;
	double score = 0;
	while (fin >> s) {
		s = simplify(s);
		if (!s.empty() && s[0] == '#' && s.substr(1) == key)
			score += 1;
	}

	fin.close();
	return score;
}

double scoreNumbersRange(string filePath, double l, double r)
{
	ifstream fin;
	fin.open(filePath);

	if (!fin.is_open()) {
		cerr << "Can't open file: " << filePath << endl;
		return 0.0;
	}

	string s;
	double score = 0;
	while (fin >> s) {
		if (!s.empty() && '0' <= s[0] && s[0] <= '9') {
			Number num = strToNumber(s);
			if (num.type == NumberType::NORMAL && l <= num.x && num.x <= r)
				score += 1;
		}
	}

	fin.close();
	return score;
}

double scorePricesRange(string filePath, double l, double r)
{
	ifstream fin;
	fin.open(filePath);

	if (!fin.is_open()) {
		cerr << "Can't open file: " << filePath << endl;
		return 0.0;
	}

	string s;
	double score = 0;
	while (fin >> s) {
		if (!s.empty() && s[0] == '$') {
			Number num = strToNumber(s);
			if (num.type == NumberType::DOLLAR && l <= num.x && num.x <= r)
				score += 1;
		}
	}

	fin.close();
	return score;
}

const double RELATIVE_ERROR = 0.05;

double getTokenScore(string filePath, Synonym & synonym, const Token & token)
{
	switch (token.op) {
	case INTITLE:
		return scoreIntitle(filePath, token.s);
	case FILE_TYPE:
		return scoreFileType(filePath, token.s);
	case SYNONYM:
		return scoreSynonym(filePath, synonym, token.s);
	case HASHTAG:
		return scoreHashtag(filePath, token.s);
	case SEARCH:
		return scoreMatch(filePath, token.s);
	case NUMBER_RANGE:
		return scoreNumbersRange(filePath, token.lo.x, token.hi.x);
	case PRICE_RANGE:
		return scorePricesRange(filePath, token.lo.x, token.hi.x);
	case PRICE:
		return scorePricesRange(filePath, token.lo.x*(1 - RELATIVE_ERROR), token.hi.x*(1 + RELATIVE_ERROR));
	default:
		return 0.0;
	}
}

double getFileScore(string filePath, Synonym & synonym, const vector<Token>& tokens)
{
	stack<double> st;

	for (Token t : tokens) {
		if (isBinaryOperator(t.op)) {
			double a = st.top(); st.pop();
			double b = st.top(); st.pop();

			double c = a + b;

			st.push(c);
		}
		else if (t.op == SearchOperator::NOT) {
			double a = st.top(); st.pop();
			st.push(-a);
		}
		else {
			st.push(getTokenScore(filePath, synonym, t));
		}
	}

	return st.top();
}
