/*
3:00 PM
7/13/2018
NTVThuyen
*/
#pragma once
#ifndef _gui_h_
#define _gui_h_

#include <string>
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <cstdlib>
#include "history.h"
#include <algorithm>
#include "Index.h"
#include "SearchResult.h"
#include <fstream>
typedef int action;

#define CERROR 12
#define CTEXT 14
#define CBTN 170
#define CINPUTFIELD 47
#define LIGHTBLUE 27
#define WHITE 8
#define YELLOW 174
#define RED 108
#define GREEN 186
#define GREY 119
#define PURPLE 77

using namespace std;

class View {
public:
	void drawTitle();
	void cursorPosition(int column, int row);
	void ShowConsoleCursor(bool showFlag);
	void setTextColor(int color);
};

class TextView:public View{
protected:
	int maxChar;
	int column;
	int row;
	string text;
public:
	TextView();
	TextView(int column, int row);
	void setText(string text);
	string getText();
	void setColumn(int column);
	void setRow(int row);
	void show();
	void show(int color);
	int getRow();
	int getColumn();
	int getMaxChar();
	void push(char a);
	void pop();
};
class TextField :public TextView {
public:
	TextField();
	void clear() {
		cursorPosition(column, row);
		for (int i = 0; i < 95; i++) cout << " ";
	}
	void sketch(int color);
	void setMaxChar(int maxLength);
protected:
};
class ListText: public TextView{
public:
	ListText() {
		
	};
	void showList(action action);
	void setup(TextField  tf);
	void setText(vector<string> text);
	void sketch(int color);
	void deleteList();
	string getText(int i);
	int getSize();
protected:
	vector<string> list;
};

class ListResult : public TextView {
public:
	ListResult() {
		maxChar = 100;
		column = 2;
		row = 12;
	}
	void print(int column, int row, string path, vector<string> input, int chightlight, int cnormal);
	void getList(vector<SearchResult> list);
	void sketch(string input_text, int choose ,Index &index,int color = WHITE, int chightlight = 139, int cchoose = LIGHTBLUE, int ctext = 255);
protected:
	vector<SearchResult> list;
};
#endif // !_gui_h_