#include "headers/SearchResult.h"

int SearchResult::fileCount;

void SearchResult::print()
{
	cout << fileIndex << " " << score << endl;
}

vector<SearchResult> SearchResult::And(const vector<SearchResult>& a, const vector<SearchResult>& b)
{
	vector<SearchResult> c;
	int j = 0;

	for (int i = 0; i < a.size(); ++i) {
		while (j < b.size() && a[i].fileIndex > b[j].fileIndex)
			++j;
		if (j < b.size() && a[i].fileIndex == b[j].fileIndex)
			c.push_back(a[i]);		
	}

	return c;
}

vector<SearchResult> SearchResult::Or(const vector<SearchResult>& a, const vector<SearchResult>& b)
{
	vector<SearchResult> c;
	int j = 0;

	for (int i = 0; i < a.size(); ++i) {
		while (j < b.size() && a[i].fileIndex > b[j].fileIndex) 
			c.push_back(b[j++]);		
		if (j == b.size() || a[i].fileIndex < b[j].fileIndex)
			c.push_back(a[i]);
	}

	while (j < b.size()) 
		c.push_back(b[j++]);

	return c;
}

vector<SearchResult> SearchResult::Not(const vector<SearchResult>& a)
{
	vector<SearchResult> c;
	int j = 0;
	for (int i = 0; i < fileCount; ++i) {
		while (j < a.size() && a[j].fileIndex < i)
			++j;
		if (j == a.size() || a[j].fileIndex > i)
			c.push_back(SearchResult(i));
	}

	return c;
}

void printResults(const vector<SearchResult> &results) {
	for (SearchResult result : results)
		result.print();
	cout << endl;
}

