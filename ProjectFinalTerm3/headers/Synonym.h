#ifndef SYNONYM_H
#define SYNONYM_H

#include "Misc.h"

#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <set>

using namespace std;

/// <summary>
/// A structure to classify synonym words into group
/// </summary>
class Synonym {
private:
	/// <summary>
	/// Synonym groups that contains a particular word
	/// </summary>
	map<string, vector<int> > groupsMap;

	/// <summary>
	/// Groups of synonyms
	/// </summary>
	vector<vector<string> > group;
public:
	/// <summary>
	/// Build and classify synonym words into group
	/// </summary>
	void buildSynonym(string dir = "synonym.txt");

	/// <summary>
	/// Load and classify synonym words into group
	/// </summary>
	void loadSynonym(string dir = "synonym.txt");

	/// <summary>
	/// Save the synonym into another file
	/// </summary>
	void saveSynonym(string dir = "synonym.txt");

	/// <summary>
	/// Return the synonyms of a word
	/// </summary>
	vector<string> getSynonym(const string &word);
};

#endif // !SYNONYM_H
