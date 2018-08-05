#ifndef SEARCH_FUNCTION_H
#define SEARCH_FUNCTION_H

#include "SearchResult.h"
#include "Index.h"
#include "Token.h"
#include "Synonym.h"

using namespace std;
#include <fstream>

/// <summary>
/// Check if key is in the title of the file with directory filePath.
/// </summary>
bool checkIntitle(string filePath, string key);

/// <summary>
/// Check if filetype of the file with directory filePath is key.
/// </summary>
bool checkFileType(string filePath, string key);

/// <summary>
/// Check if the file with directory filePath contain word with synonym meaning to key.
/// </summary>
bool checkSynonym(string filePath, Synonym &synonym, string key);

/// <summary>
/// Check if key matches the content of the file with directory filePath.
/// </summary>
bool exactMatch(string filePath, string key);

/// <summary>
/// Check if the file with directory filePath contain hashtag key.
/// </summary>
bool checkHashtag(string filePath, string key);

/// <summary>
/// Check if the file with directory filePath contain number within range [l, r].
/// </summary>
bool checkNumbersRange(string filePath, double l, double r);

/// <summary>
/// Check if the file with directory filePath contain price within range [l, r].
/// </summary>
bool checkPricesRange(string filePath, double l, double r);

/// <summary>
/// Return a list of SearchResult for all the files in the index satisfying the token
/// </summary>
vector<SearchResult> search(Index &index, Synonym &synonym, const Token &token);

#endif // !SEARCH_FUNCTION_H

