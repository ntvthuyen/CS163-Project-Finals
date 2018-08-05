#ifndef MISC_H
#define MISC_H

#include <string>
#include <vector>

using namespace std;

/// <summary>
/// Split string s into a list of word, seperated by the delimeter.
/// </summary>
vector<string> split(string s, char delimeter = ' ');

/// <summary>
/// Simplify a word by lowercase all the character, removing dot, comma, etc.
/// </summary>
string simplify(string word);

/// <summary>
/// Convert a string to lowercasze
/// </summary>
string toLower(string s);

#endif MISC_H
