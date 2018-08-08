#include "headers/gui.h"
#include "headers/search_activity.h"
#include "headers/Index.h"
#include "headers/Synonym.h"
#include <ctime>
int main() {
	//clock_t d;
	//d = clock();
	
	View w;	
	Synonym synonym;
	w.drawTitle();
	w.cursorPosition(10, 20); cout << "Nguyen Truong Vinh Thuyen";
	w.cursorPosition(10, 21); cout << "Nguyen Diep Xuan Quang";
	w.cursorPosition(10, 22); cout << "Tran Bao Phuc";
	w.cursorPosition(10, 23); cout << "Pham Minh Tuan";
	w.cursorPosition(25, 7); cout << "LOADING...";
	w.ShowConsoleCursor(false);

	Index index;
	if (index.checkChange("D:\\CS163_Project_Data\\CS163_Project_Data"))
		index.buildIndex("D:\\CS163_Project_Data\\CS163_Project_Data");
	else
		index.loadIndex();
	
	synonym.loadSynonym();
	
	SearchResult::fileCount = index.getFileCount();
	
	w.cursorPosition(25, 7); cout << "			 ";

//	cout << clock() - d;
	TextField tf;
	tf.setColumn(10);
	tf.setMaxChar(95);
	tf.setRow(9);
	input(tf, YELLOW);
	activity2( tf.getText(), index, synonym);
	system("pause");
	return 0;
}