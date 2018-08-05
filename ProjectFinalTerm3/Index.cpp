#include "headers/Index.h"
#include "headers/Misc.h"
#include "headers/Synonym.h"
int Index::addWordGetNode(const string & word)
{
	int node = 0;
	map<char, int>::iterator it;
	for (char ch : word) {
		it = tree[node].nxt.find(ch);
		if (it == tree[node].nxt.end()) {
			tree.push_back(Node());
			it = tree[node].nxt.insert({ ch, tree.size() - 1}).first;
		}
		node = it->second;
	}
	return node;
}

void Index::addWord(const int &fileIndex, const string &word) {
	int node = addWordGetNode(word);
	if (tree[node].occurence.empty() || tree[node].occurence.back() != fileIndex)
		tree[node].occurence.push_back(fileIndex);
}

void Index::addWord(const vector<int>& fileIndices, const string &word)
{
	int node = addWordGetNode(word);
	tree[node].occurence.assign(fileIndices.begin(), fileIndices.end());
}

void Index::saveToStream(int node, string &word, ofstream &fout) {
	if (tree[node].occurence.size() > 0) {
		fout << word << " " << tree[node].occurence.size();
		for (int fileIndex : tree[node].occurence)
			fout << " " << fileIndex;
		fout << endl;
	}

	for (pair<char, int> p : tree[node].nxt) {
		word += p.first;
		saveToStream(p.second, word, fout);
		word.pop_back();
	}
}

void Index::addFromFile(fs::path filePath) {
	ifstream fin;
	fin.open(filePath);
	if (!fin.is_open()) {
		cerr << "File not found: " << filePath;
		return;
	}

	int fileIndex = filePaths.size();
	filePaths.push_back(filePath.string());

	string word;
	bool haveDollar = false;
	bool haveHashtag = false;
	vector<bool> haveDigit(10, false);

	while (fin >> word) {
		word = simplify(word);
		if (word == "")
			continue;
		if (word[0] == '$')
			haveDollar = true;
		if (word[0] == '#')
			haveHashtag = true;
		if (isdigit(word[0]))
			haveDigit[word[0] - '0'] = true;
		addWord(fileIndex, word);
	}

	if (haveDollar)
		addWord(fileIndex, "$");
	if (haveHashtag)
		addWord(fileIndex, "#");
	for (int d = 0; d < 10; ++d) {
		if (haveDigit[d]) {
			string digitStr; 
			digitStr += '0' + d;
			addWord(fileIndex, digitStr);
		}
	}

	fin.close();
}

void Index::addFromDirectory(string dirPath) {
	fs::path dir(dirPath.c_str());

	if (fs::exists(dir) && fs::is_directory(dir)) {
		for (const auto &entry : fs::directory_iterator(dir)) {
			if (fs::is_regular_file(entry.status()))
				addFromFile(entry.path());
		}
	}
	else
		cerr << "Directory not found: " << dir << endl;
}

void Index::buildIndex(string dirPath, string outputPath) {
	addFromDirectory(dirPath);
	saveIndex(outputPath);
}

void Index::loadIndex(string inputPath)
{
	ifstream fin;
	fin.open(inputPath);
	if (!fin.is_open()) {
		cerr << "Find not found: " << inputPath << endl;
		return;
	}

	int nFile;
	fin >> nFile;
	//Remove the end of line before the first file name
	string tmp;
	getline(fin, tmp);

	filePaths.resize(nFile);
	for (int i = 0; i < nFile; ++i)
		getline(fin, filePaths[i]);

	vector<int> fileIndices;
	while (!fin.eof()) {
		string word;
		fin >> word;
		if (word == "")
			continue;

		int nIndices;
		fin >> nIndices;
		fileIndices.resize(nIndices);
		for (int i = 0; i < nIndices; ++i)
			fin >> fileIndices[i];
		addWord(fileIndices, word);
	}
	fin.close();
}

void Index::saveIndex(string outputPath)
{
	ofstream fout;
	fout.open(outputPath);
	if (!fout.is_open()) {
		cerr << "Can't open file: " << outputPath << endl;
		return;
	}

	fout << filePaths.size() << endl;
	for (int i = 0; i < filePaths.size(); ++i)
		fout << filePaths[i] << endl;

	string word = "";
	saveToStream(0, word, fout);

	fout.close();
}

vector<SearchResult> Index::getFilesContainWord(const string &word)
{
	vector<SearchResult> res;

	int node = 0;
	map<char, int>::iterator it;
	for (char ch : word) {
		it = tree[node].nxt.find(ch);
		if (it == tree[node].nxt.end())
			return res;
		node = it->second;
	}

	for (int fileIndex : tree[node].occurence)
		res.push_back(SearchResult(fileIndex));
	return res;
}

string Index::getFilePath(int fileIndex)
{
	return filePaths[fileIndex];
}

Index::Index()
{
	tree.push_back(Node());
}

Index::~Index() {
	tree.clear();
	filePaths.clear();
}

int Index::getFileCount()
{
	return filePaths.size();
}

inline Index::Node::~Node() {
	nxt.clear();
	occurence.clear();
}
