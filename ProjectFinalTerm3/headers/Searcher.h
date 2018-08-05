#ifndef SEARCHER_H
#define SEARCHER_H

#include "SearchFunction.h"
#include "Token.h"
#include "Synonym.h"
#include "RankFunction.h"
//#include "Misc.h"
#include <stack>

/// <summary>
/// Return a list of tokens with regular notation (infix notation) from a query.
/// </summary>
vector<Token> preProcess(const string & query);

/// <summary>
/// Convert a list of tokens from infix notation to reverse Polish notation.
/// </summary>
vector<Token> getRPN(const vector<Token> &tokens);

/// <summary>
/// Evaluate the tokens list. Return a list of files satisfying the tokens list.
/// </summary>
vector<SearchResult> evaluate(Index & index, Synonym &synonym, const vector<Token> &tokens);

/// <summary>
/// Evaluate the score of all the files and return top 5 files with highest score from a list
/// </summary>
vector<SearchResult> getTop5Score(Index & index, Synonym &synonym, vector<SearchResult> results, const vector<Token>& tokens);

/// <summary>
/// Process the query from the user
/// </summary>
vector<SearchResult> processQuery(Index & index, Synonym &synonym, string query);

#endif // !SEARCHER_H

