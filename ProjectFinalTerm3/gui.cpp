#include "headers/gui.h"
void View::cursorPosition(int column, int row) {

	COORD point;
	point.X = column; point.Y = row;
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), point);
}
void View::ShowConsoleCursor(bool showFlag){
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);

	CONSOLE_CURSOR_INFO     cursorInfo;

	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = showFlag; // set the cursor visibility
	SetConsoleCursorInfo(out, &cursorInfo);
}
void View::drawTitle() {
	HANDLE hConsoleOutput;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
	GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);
	WORD wAttributes = screen_buffer_info.wAttributes;
	wAttributes &= 0xfff0; wAttributes |= LIGHTBLUE & 0x000f;
	SetConsoleTextAttribute(hConsoleOutput, wAttributes);
	cout << endl << endl;
	cout <<	"   |v | _   _  _   _  |    _  _  _   _  _ |_     _  _   _  .  _   _" << endl
		 << "   | v|(_) |  ||| (_| |   _) (- (_| |  (_ | )   (- | ) (_) | | ) (-" << endl
		 << "                                                       _/          " << endl;

}
void View::setTextColor(int color) {
	HANDLE hConsoleOutput;
	hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_SCREEN_BUFFER_INFO screen_buffer_info;
	GetConsoleScreenBufferInfo(hConsoleOutput, &screen_buffer_info);
	WORD wAttributes = screen_buffer_info.wAttributes;
	color &= 0x000f;
	wAttributes &= 0xfff0; wAttributes |= color;
	SetConsoleTextAttribute(hConsoleOutput, wAttributes);
}
TextView::TextView() {
	text = "";
	column = 0;
	row = 0;
};
TextView::TextView(int column, int row){
	this->column = column;
	this->row = row;
}
void TextView::setColumn(int column) {
	this->column = column;
}
void TextView::setRow(int row) {
	this->row = row;
}
void TextView::setText(string text) {
	this->text = text;
}
string TextView::getText() {
	return text;
}
int TextView::getColumn() {
	return column;
}
int TextView::getRow() {
	return row;
}
void TextView::show() {
	View::cursorPosition(column, row);
	cout << text;
}
void TextView::show(int color) {
	setTextColor(color);
	show();
}
void TextView::push(char a) {
	if (maxChar >= text.length())
	text.push_back(a);
}
int TextView::getMaxChar() {
	return maxChar;
}
void TextView::pop() {
	text.pop_back();
}
TextField::TextField() {
	maxChar = 0;
	column = 0;
	row = 0;
}
void TextField::setMaxChar(int maxLength) {
	maxChar = maxLength;
}
void TextField::sketch(int color) {
	setTextColor(color);
	int right = column + maxChar + 1;
	int top = row - 1;
	int bottom = row + 1;
	int left = column - 1 ;
	cursorPosition(left, top); cout << char(201);
	cursorPosition(right, top); cout << char(187);
	cursorPosition(left, bottom); cout << char(200);
	cursorPosition(right, bottom); cout << char(188);
	for (int i = left + 1; i <= right - 1; i++) {
		cursorPosition(i, top); cout << char(205);
		cursorPosition(i, bottom); cout << char(205);
	}
	for (int i = top + 1; i <= bottom - 1; i++) {
		cursorPosition(left, i); cout << char(186);
		cursorPosition(right, i); cout << char(186);
	}
	cursorPosition(column, row);
}
void ListText::sketch(int color) {
	int length = list.size();
	setTextColor(color);
	int right = column + maxChar + 1;
	int top = row - 1;
	int bottom = row + length;
	int left = column - 1;

	cursorPosition(left, bottom); cout << char(200);
	cursorPosition(right, bottom); cout << char(188);
	for (int i = left + 1; i <= right - 1; i++) {
		cursorPosition(i, bottom); cout << char(205);
	}
	for (int i = top + 1; i <= bottom - 1; i++) {
		cursorPosition(left, i); cout << char(186);
		cursorPosition(right, i); cout << char(186);
	}
	cursorPosition(column, row);
}
void ListText::setText(vector<string> text) {
	list = text;
}
void ListText::deleteList() {
	for (int i = 0; i < 6; i++) {
		cursorPosition(column + 1, row + i);
		cout << "                                                                                                                                          ";
	}
}

void ListText::setup(TextField tf) {
	maxChar = tf.getMaxChar();
	column = tf.getColumn();
	row = tf.getRow()+2;
}
void ListText::showList(action action) {
	if (action > 0) {
		setTextColor(WHITE);
		for (int i = 0; i < list.size(); i++) {
			cursorPosition(column, row + i);
			cout << list[i];
		}
		cursorPosition(column, row + action - 1);
		setTextColor(LIGHTBLUE);
		cout << list[action - 1];
		return;
	}
	deleteList();
	sketch(WHITE);
	for (int i = 0; i < list.size(); i++) {
		cursorPosition(column, row + i);
		cout << list[i];
	}
	
}
int ListText::getSize() {
	return list.size();
}
string ListText::getText(int i) {
	if (list.size() > i) {
		return list[i];
	}
    return NULL;
}
void ListResult::getList(vector<SearchResult> list) {
	this->list = list;
}
void ListResult::print(int column, int row,string path, vector<string> input, int chightlight, int cnormal) {
	cursorPosition(column, row);
	bool check = false;
	string text;
	string temp;
	ifstream fin;
	fin.open(path);
	getline(fin, text);
	vector<string> title;
	int countCh = 0;
	int _row = 1;

	for (int i = 0; i < text.length(); i++) {
		if (text[i] != ' ') {
			temp.push_back(text[i]);
		}
		else {
			title.push_back(temp);
			temp = "";
		}
	}
	setTextColor(LIGHTBLUE);
	cout << "<";
	for (int i = 0; i < title.size(); i++) {
		check = false;
		for (int j = 0; j < input.size(); j++) {
			temp = title[i];
			transform(temp.begin(), temp.end(), temp.begin(), ::tolower);
			if (temp == input[j]) {
				setTextColor(chightlight);
				if (countCh > 100) {
					_row++;
					cursorPosition(column, row + _row);
					countCh = 0;
				}
				cout << title[i] << " ";
				check = true;
				break;
			}
		}
		if (!check) {
			setTextColor(cnormal);
			if (countCh > 100) {
				_row++;
				cursorPosition(column, row + _row);
				countCh = 0;
			}	
			cout << title[i] << " ";

		}
	}
	setTextColor(LIGHTBLUE);
	cout << ">";
	
	cursorPosition(column, row + _row);
	setTextColor(cnormal);
	cout << "...";
	while (fin >> temp) {
		for (int j = 0; j < input.size(); j++) {
			string _temp = temp;
			transform(_temp.begin(), _temp.end(), _temp.begin(), ::tolower);
			if (_temp == input[j]) {
				countCh += temp.length();
				setTextColor(chightlight);
				if (countCh > 100) {
					_row++;
					cursorPosition(column, row + _row);
					countCh = 0;
				}
				cout << temp << " ";
				check = true;
				break;
			}
		}
		if (check) { break; }
	}
	for (int i = 0; i <= 30 && !fin.eof(); i++) {
		fin >> temp;
		check = false;
		for (int j = 0; j < input.size(); j++) {
			string _temp = temp;
			transform(_temp.begin(), _temp.end(), _temp.begin(), ::tolower);
			if (_temp == input[j]) {
				countCh += temp.length();
				setTextColor(chightlight);
				if (countCh > 100) {
					_row++;
					cursorPosition(column, row + _row);
					countCh = 0;
				}	
				cout << temp << " ";
				check = true;
				break;
			}
		}
		if (!check) {
			setTextColor(cnormal);			
			countCh += temp.length();
			if (countCh > 100) {
				_row++;
				cursorPosition(column, row + _row);
				countCh = 0;
			}
			cout << temp << " ";

		}
		
	}
	setTextColor(cnormal);
	cout << "...";
	fin.close();
}
void ListResult::sketch(string input_text, int choose, Index &index, int color, int chightlight, int cchoose, int ctext) {
	system("CLS");
	cursorPosition(column, row);
	vector<string> input;
	string temp;
	for (int i = 0; i < input_text.length(); i++) {
		if (input_text[i] != ' ') {
			temp.push_back(tolower(input_text[i]));
		}
		else {
			input.push_back(temp);
			//cout << temp << endl;
			temp = "";
		}
		
	}input.push_back(temp);

	for (int i = 0; i < list.size(); i++) {
		int n = list[i].fileIndex;
		print(column,row + i * 7, index.getFilePath(n), input, chightlight, ctext);
	}
}
