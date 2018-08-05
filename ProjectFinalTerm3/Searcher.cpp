#include "headers/Searcher.h"

/*
Example:
1) computer science OR " informatic technology"
Preprocess: {computer, AND, science, OR, " informatic technology"}
2) computer (intitle:science OR filetype:txt)
Preprocess: {computer, AND, (, intitle:science, OR, txt, )}
3) Peanut Butter +and Jam $100
Preprocess: {Peanut, AND, Butter, AND, and , AND, Jam, AND, $100}
4) #Manchester -united
Preprocess: {#Manchester, AND, NOT, united}
*/

bool addAnd(SearchOperator a, SearchOperator b) {
	if (isBinaryOperator(a) || isBinaryOperator(b))
		return false;
	if (a == SearchOperator::OPEN_BRACKET || b == SearchOperator::CLOSE_BRACKET)
		return false;
	if (a == SearchOperator::NOT)
		return false;
	return true;
}

void addToken(vector<Token> &tokens, const Token &token) {
	if (!tokens.empty() && addAnd(tokens.back().op, token.op))
		tokens.push_back(Token("AND", SearchOperator::AND));
	tokens.push_back(token);
}

vector<Token> preProcess(const string & query) {
	vector<Token> tokens;
	string curWord = "";
	bool inDoubleQuote = false;

	for (int i = 0; i <= query.length(); ++i) {
		if (i == query.length() || (query[i] == ' ' && !inDoubleQuote)) {
			if (curWord != "")
				addToken(tokens, getToken(curWord));
			curWord = "";
		}
		else if (query[i] == '"') {
			if (inDoubleQuote) {
				if (curWord != "") 
					addToken(tokens, Token(curWord, SearchOperator::SEARCH));				
				curWord = "";
			}
			else {
				if (curWord != "")
					addToken(tokens, getToken(curWord));
				curWord = "";
			}
			inDoubleQuote ^= true;
		}
		else if (inDoubleQuote) {
			curWord += query[i];
		}
		else if (query[i] == '(' || query[i] == ')') {
			if (curWord != "")
				addToken(tokens, getToken(curWord));
			curWord = "";
			addToken(tokens, getToken(string(1, query[i])));
		}
		else if (query[i] == '-') {
			if (curWord != "")
				addToken(tokens, getToken(curWord));
			curWord = "";
			addToken(tokens, Token("NOT", SearchOperator::NOT));
		}
		else {
			curWord += query[i];
		}
	}

	return tokens;
}

vector<Token> getRPN(const vector<Token> &tokens)
{
	stack<Token> st;
	string curWord;
	vector<Token> rpn;

	for (Token t : tokens) {
		if (isFunctionOperator(t.op)) {
			rpn.push_back(t);
		}
		else if (isLogicalOperator(t.op)) {
			while (!st.empty() && st.top().op != SearchOperator::OPEN_BRACKET && (isFunctionOperator(st.top().op) || precedence(st.top().op) >= precedence(t.op))) {
				rpn.push_back(st.top());
				st.pop();
			}
			st.push(t);
		}
		else if (t.op == SearchOperator::OPEN_BRACKET) {
			st.push(t);
		}
		else if (t.op == SearchOperator::CLOSE_BRACKET) {
			while (!st.empty() && st.top().op != SearchOperator::OPEN_BRACKET) {
				rpn.push_back(st.top());
				st.pop();
			}
			if (!st.empty())
				st.pop();
		}
	}

	while (!st.empty()) {
		if (st.top().op == SearchOperator::OPEN_BRACKET)
			break;
		rpn.push_back(st.top());
		st.pop();
	}

	return rpn;
}

vector<SearchResult> evaluate(Index & index, Synonym &synonym, const vector<Token> &tokens) {
	stack<vector<SearchResult> > st;
	for (Token t : tokens) {
		if (isBinaryOperator(t.op)) {
			vector<SearchResult> a = st.top(); st.pop();
			vector<SearchResult> b = st.top(); st.pop();

			vector<SearchResult> c;
			if (t.op == SearchOperator::AND)
				c = SearchResult::And(a, b);
			else
				c = SearchResult::Or(a, b);

			st.push(c);
		}
		else if (t.op == SearchOperator::NOT) {
			vector<SearchResult> a = st.top(); st.pop();
			st.push(SearchResult::Not(a));
		}
		else {
			st.push(search(index, synonym, t));
		}		
	}
	
	return st.top();
}

bool descScore(const SearchResult &a, const SearchResult &b) {
	return a.score > b.score;
}

vector<SearchResult> getTop5Score(Index & index, Synonym & synonym, vector<SearchResult> results, const vector<Token>& tokens)
{
	for (int i = 0; i < results.size(); ++i) 
		results[i].score = getFileScore(index.getFilePath(results[i].fileIndex), synonym, tokens);
	sort(results.begin(), results.end(), descScore);

	while (results.size() > 5)
		results.pop_back();
	return results;
}

vector<SearchResult> processQuery(Index & index, Synonym &synonym, string query)
{
	vector<Token> pre = preProcess(query);
	//printTokens(pre);
	vector<Token> rpn = getRPN(pre);
	//printTokens(rpn);
	vector<SearchResult> results = evaluate(index, synonym, rpn);

	results = getTop5Score(index, synonym, results, rpn);

	return results;
}

