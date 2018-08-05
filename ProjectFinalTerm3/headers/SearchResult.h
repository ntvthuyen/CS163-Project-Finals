#ifndef SEARCH_RESULT_H
#define SEARCH_RESULT_H

#include <string>
#include <vector>
#include <iostream>

using namespace std;

/// <summary>
/// Representing a results from the search engine.
/// </summary>
struct SearchResult {
	/// <summary>
	/// Index of the file.
	/// </summary>
	int fileIndex;

	/// <summary>
	/// Score of the file. Higher score mean higher priority.
	/// </summary>
	int score;

	/// <summary>
	/// The number of files in the index
	/// </summary>
	static int fileCount;

	SearchResult(int fileIndex) : fileIndex(fileIndex), score(0) {}	

	/// <summary>
	/// Print the search result
	/// </summary>
	void print();
	
	/// <summary>
	/// Get the intersection of search results
	/// </summary>
	static vector<SearchResult> And(const vector<SearchResult> &a, const vector<SearchResult> &b);

	/// <summary>
	/// Get the union of search results
	/// </summary>
	static vector<SearchResult> Or(const vector<SearchResult> &a, const vector<SearchResult> &b);
	/// <summary>
	/// Get the complement of search results
	/// </summary>
	static vector<SearchResult> Not(const vector<SearchResult> &a);
};

/// <summary>
/// Print the search results array
/// </summary>
void printResults(const vector<SearchResult> &results);

/// <summary>
/// Maximum number of words in a phrase that an asterisk can replace
/// </summary>
const int MAX_ASTERISK_WORD_COUNT = 3;

#endif SEARCH_RESULT_H
