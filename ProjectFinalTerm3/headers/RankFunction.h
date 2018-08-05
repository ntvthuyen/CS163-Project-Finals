#ifndef RANK_FUNCTION_H
#define RANK_FUNCTION_H

#include "SearchResult.h"
#include "Token.h"
#include "Synonym.h"
#include <vector>
#include <algorithm>
#include <stack>

using namespace std;
#include <fstream>

/// <summary>
/// Give the score for the file with directory filePath: key is in the title.
/// </summary>
double scoreIntitle(string filePath, string key);

/// <summary>
/// Give the score for the file with directory filePath: filetype is key.
/// </summary>
double scoreFileType(string filePath, string key);

/// <summary>
/// Give the score for the file with directory filePath: word with synonym meaning to key.
/// </summary>
double scoreSynonym(string filePath, Synonym &synonym, string key);

/// <summary>
/// Give the score for the file with directory filePath: directory filePath.
/// </summary>
double scoreMatch(string filePath, string key);

/// <summary>
/// Give the score for the file with directory filePath: hashtag key.
/// </summary>
double scoreHashtag(string filePath, string key);

/// <summary>
/// Give the score for the file with directory filePath: number within range [l, r].
/// </summary>
double scoreNumbersRange(string filePath, double l, double r);

/// <summary>
/// Give the score for the file with directory filePath: price within range [l, r].
/// </summary>
double scorePricesRange(string filePath, double l, double r);

/// <summary>
/// Give the score for the file based on the token
/// </summary>
double getTokenScore(string filePath, Synonym &synonym, const Token &token);

/// <summary>
/// Give the score for the file based on the token expression
/// </summary>
double getFileScore(string filePath, Synonym &synonym, const vector<Token> &tokens);

#endif // !RANK_FUNCTION_H

