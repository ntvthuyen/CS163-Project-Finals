#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <iostream>
#include <vector>
#include "Number.h"
#include "Misc.h"

using namespace std;

/// <summary>
/// All the search operator in the search engine
/// </summary>
enum SearchOperator {
	// Logic operator
	AND,
	OR,
	NOT,

	// Bracket operator
	OPEN_BRACKET,
	CLOSE_BRACKET,

	// Prefix operator
	PRICE,
	HASHTAG,
	SYNONYM,
	FILE_TYPE,
	INTITLE,

	// Range operator
	NUMBER_RANGE,
	PRICE_RANGE,

	// Match operator
	SEARCH,

	// Misc
	EMPTY,
};

/// <summary>
/// A token used in the reverse polish notation (RPN), representing a operand
/// </summary>
struct Token {
	/// <summary>
	/// String content of the token (in search, intitle, filetype, etc. operator)
	/// </summary>
	string s;

	/// <summary>
	/// Number range of the token (in price range and number range operator)
	/// </summary>
	Number lo, hi;

	/// <summary>
	/// Operator of the token
	/// </summary>
	SearchOperator op;

	Token(string s, SearchOperator op, Number lo = Number(), Number hi = Number()) : s(s), op(op), lo(lo), hi(hi) {}

	/// <summary>
	/// Print the content and operator of the token
	/// </summary>
	void print();
};

/// <summary>
/// Check if a SearchOperator is a binary logical operator (AND, OR)
/// </summary>
bool isBinaryOperator(SearchOperator op);

/// <summary>
/// Check if a SearchOperator is a logical operator (AND, OR, NOT)
/// </summary>
bool isLogicalOperator(SearchOperator op);

/// <summary>
/// Check if a SearchOperator is a function operator (all except AND, OR, NOT, OPEN_BRACKET, CLOSE_BRACKET)
/// </summary>
bool isFunctionOperator(SearchOperator op);


/// <summary>
/// Convert the content of a String to a Token
/// </summary>
Token getToken(string s);

/// <summary>
/// Print a list of Token
/// </summary>
void printTokens(const vector<Token> &tokens);

/// <summary>
/// Return the precedence of a logical SearchOperator (NOT > AND > OR)
/// </summary>
int precedence(SearchOperator op);

#endif // !TOKEN_H
