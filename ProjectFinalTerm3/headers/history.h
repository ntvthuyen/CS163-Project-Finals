#pragma once
#ifndef _history_h_
#define _history_h_

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

bool isPrefix(string text, string pattern);
vector<string> getRecent(string input, int resultCount = 5 ,string path = "log.txt");
void logFile(string str, string path = "log.txt");
vector<string> _getRecent(string path = "log.txt");
#endif // 

