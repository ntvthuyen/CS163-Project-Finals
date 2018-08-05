#include "headers/Token.h"

void Token::print()
{
	if (op == SearchOperator::SEARCH)
		cout << "'" << s << "'";
	else if (op == SearchOperator::PRICE)
		cout << "$" << s;
	else if (op == SearchOperator::PRICE_RANGE)
		cout << "$" << lo.x << "-" << hi.x;
	else if (op == SearchOperator::NUMBER_RANGE)
		cout << lo.x << "-" << hi.x;
	else if (op == SearchOperator::HASHTAG)
		cout << "#" << s;
	else if (op == SearchOperator::INTITLE)
		cout << "intitle:" << s;
	else if (op == SearchOperator::FILE_TYPE)
		cout << "filetype:" << s;
	else if (op == SearchOperator::SYNONYM)
		cout << "~" << s;
	else
		cout << "[" << s << "]";
	cout << " ";
}

bool isBinaryOperator(SearchOperator op)
{
	return (op == SearchOperator::AND || op == SearchOperator::OR);
}

bool isLogicalOperator(SearchOperator op)
{
	return (isBinaryOperator(op) || op == SearchOperator::NOT);
}

bool isFunctionOperator(SearchOperator op) {
	return (!isLogicalOperator(op) && op != SearchOperator::OPEN_BRACKET && op != SearchOperator::CLOSE_BRACKET);
}

int precedence(SearchOperator op) {
	if (op == SearchOperator::NOT) return 2;
	if (op == SearchOperator::AND) return 1;
	if (op == SearchOperator::OR) return 0;
	return -1;
}

Token getToken(string s) {	
	if (s.empty())
		return Token("", SearchOperator::EMPTY);

	if (s == "AND")
		return Token("AND", SearchOperator::AND);
	if (s == "OR")
		return Token("OR", SearchOperator::OR);
	if (s == "NOT")
		return Token("NOT", SearchOperator::NOT);
	if (s == "(")
		return Token("(", SearchOperator::OPEN_BRACKET);
	if (s == ")")
		return Token(")", SearchOperator::CLOSE_BRACKET);

	size_t pos = s.find('.');
	if (pos != string::npos && s.length() > pos + 1 && s[pos + 1] == '.') {
		string strLo = s.substr(0, pos);
		string strHi = s.substr(pos + 2);
		Number numLo = strToNumber(strLo);
		Number numHi = strToNumber(strHi);
		if (numLo.type != NumberType::INVALID && numLo.type != NumberType::INVALID) {
			if (numLo.type == NumberType::DOLLAR || numHi.type == NumberType::DOLLAR)
				return Token(s, SearchOperator::PRICE_RANGE, numLo.x, numHi.x);
			else
				return Token(s, SearchOperator::NUMBER_RANGE, numLo.x, numHi.x);
		}			
	}	

	if (s[0] == '$') {
		Number num = strToNumber(s);
		if (num.type == NumberType::DOLLAR)
			return Token(s.substr(1), SearchOperator::PRICE, num, num);
	}
	if (s[0] == '#')
		return Token(s.substr(1), SearchOperator::HASHTAG);
	if (s[0] == '~')
		return Token(s.substr(1), SearchOperator::SYNONYM);
	if (s[0] == '+')
		return Token(s.substr(1), SearchOperator::SEARCH);

	if (s.length() > 8 && s.substr(0, 8) == "intitle:")
		return Token(s.substr(8), SearchOperator::INTITLE);
	if (s.length() > 9 && s.substr(0, 9) == "filetype:")
		return Token(s.substr(9), SearchOperator::FILE_TYPE);

	return Token(s, SearchOperator::SEARCH);
}

void printTokens(const vector<Token> &tokens) {
	for (Token token : tokens)
		token.print();
	cout << endl;
}