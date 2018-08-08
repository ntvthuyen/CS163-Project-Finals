#ifndef TRIE_H
#define TRIE_H

#include "SearchResult.h"
#include <unordered_map>
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <experimental/filesystem> // C++-standard header file name
#include <filesystem> // Microsoft-specific implementation header file name

using namespace std;
namespace fs = std::experimental::filesystem;

/// <summary> 
/// The index of all the input file. Main functionality is to list indices of all files containing a certain word.
/// </summary>
class Index {
private:
	/// <summary> 
	/// Node structure for the trie.
	/// </summary>
	static struct Node {
		/// <summary> 
		/// Position of the next node going from current node via a certain character.
		/// </summary>
		map<char, int> nxt;

		/// <summary> 
		/// Indices of all files containing the word represented by the node.
		/// </summary>
		vector<int> occurence;

		~Node();
	};

	/// <summary> 
	/// The trie structure.
	/// </summary>
	vector<Node> tree;

	/// <summary> 
	/// All the file paths in the index.
	/// </summary>
	vector<string> filePaths;

	/// <summary> 
	/// Helper function for saveIndex function.
	/// </summary>
	void saveToStream(int node, string &word, ofstream &fout);

	/// <summary> 
	/// Inputing the content from filePath to the index.
	/// </summary>
	void addFromFile(fs::path filePath);

	/// <summary> 
	/// Add the word to the index. Return the position of the node representing the word.
	/// </summary>
	int addWordGetNode(const string &word);

	/// <summary> 
	/// Add a word to the index including the index of the file containing that word. 
	/// </summary>
	void addWord(const int &fileIndex, const string &word);

	/// <summary> 
	/// Add a word to the index including indices of files containing that word. 
	/// </summary>
	void addWord(const vector<int> &fileIndices, const string &word);

	/// <summary> 
	/// Inputing the content from all files from directory dir to the index.
	/// </summary>
	void addFromDirectory(string dir);

public:
	Index();

	~Index();

	/// <summary> 
	/// Return the number of files in the index
	/// </summary>
	int getFileCount();
	bool checkChange(string dirPath, string inputPath = "index.txt");

	/// <summary> 
	/// Build the index by inputing all the files from directoty dir, then save the index content to outputPath
	/// </summary>
	void buildIndex(string dir, string outputPath = "index.txt");

	/// <summary> 
	/// Load the file paths and content of the index from the inputPath
	/// </summary>
	void loadIndex(string inputPath = "index.txt");

	/// <summary> 
	/// Save the file paths and content of the index to the outputPath
	/// </summary>
	void saveIndex(string outputPath = "debugIndex.txt");

	/// <summary> 
	/// Return a list of indices of files containing the string word in it's content
	/// </summary>
	vector<SearchResult> getFilesContainWord(const string &word);

	/// <summary> 
	/// Return the full path of a fileIndex-th file in the index
	/// </summary>
	string getFilePath(int fileIndex);
};

#endif TRIE_H