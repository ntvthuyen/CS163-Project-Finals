#include "headers/Synonym.h"

void Synonym::buildSynonym(string dir)
{
	ifstream fin;
	fin.open(dir);
	if (!fin.is_open()) {
		cout << "Can't open file: " << dir << endl;
		return;
	}

	string line;
	while (getline(fin, line)) {
		if (line == "")
			continue;

		vector<string> words = split(line);
		for (int i = 0; i < words.size(); ++i)
		{
			int j = 0;
			while (j < words[i].size() && words[i][j] >= 0 && words[i][j] <= 255 && isalnum(words[i][j]))
				++j;

			words[i] = simplify(words[i].substr(0, j));
			groupsMap[words[i]].push_back(group.size());
		}

		group.push_back(words);
	}

	fin.close();
}

void Synonym::loadSynonym(string dir)
{
	ifstream fin;
	fin.open(dir);
	if (!fin.is_open()) {
		cout << "Can't open file: " << dir << endl;
		return;
	}

	string line;
	while (getline(fin, line)) {
		if (line == "")
			continue;

		vector<string> words = split(line);
		for (int i = 0; i < words.size(); ++i)
		{
			words[i] = simplify(words[i]);
			groupsMap[words[i]].push_back(group.size());
		}

		group.push_back(words);
	}

	fin.close();
}

void Synonym::saveSynonym(string dir)
{
	ofstream fout;
	fout.open(dir);
	if (!fout.is_open()) {
		cout << "Can't open file: " << dir << endl;
		return;
	}

	for (int i = 0; i < group.size(); ++i) {
		for (string word : group[i])
			fout << word << " ";
		fout << endl;
	}

	fout.close();
}

vector<string> Synonym::getSynonym(const string & word)
{
	map<string, vector<int> >::iterator it = groupsMap.find(word);
	if (it == groupsMap.end())
		return vector<string>(1, word);

	set<string> synonymSet;
	for (int groupId : it->second)
		for (string word : group[groupId])
			synonymSet.insert(word);
	return vector<string>(synonymSet.begin(), synonymSet.end());
}
