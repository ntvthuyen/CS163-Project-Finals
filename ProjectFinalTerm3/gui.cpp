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
void TextView::setTextColor(int color) {
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
		cout << "                                                                                                                           ";
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