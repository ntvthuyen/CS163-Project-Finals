#include "headers/search_activity.h"
void input(TextField &tf, int color) {
	ListText lt;
	tf.sketch(LIGHTBLUE);
	int listLength = 1;
	int act = 0;
	lt.setup(tf);
	lt.setText(_getRecent());
	lt.showList(0);
	tf.show(color);
	int b;
	char a;
	while (true) {
		b = _getch();
		if (b == 224) {
			int _c = _getch();
			switch (_c) {
			case 72: {//up
				listLength = lt.getSize() + 1;
				if (act == 0) act = listLength;
				act = abs((act - 1) % listLength);
				lt.showList(act);
				break; 
			}
			case 80: { //down
				listLength = lt.getSize() + 1;
				act = (act + 1) % listLength;
				lt.showList(act);
				break;
			}
			}
		}
		else if (b == 13 ) {
			_getch();
			if (act == 0 ) {
				if (tf.getText().length() > 0) {
					logFile(tf.getText());
					tf.clear();
					tf.show(YELLOW);
					return;
				}
			}
			else {
				logFile(lt.getText(act - 1));
				tf.setText(lt.getText(act - 1));
				tf.clear();
				tf.show(YELLOW);
				return;
			}
		} // enter
		else{
			a = b;
			_getch();
			if (b != '\b') {
				act = 0;
				tf.push(a);
				lt.setText(getRecent(tf.getText()));
				lt.showList(0);
				tf.show(color);
				cout << "|";
			}
			else if (tf.getText().length() != 0) {
				act = 0;
				tf.pop();
				View view;
				lt.setText(getRecent(tf.getText()));
				lt.showList(0);
				view.cursorPosition(tf.getColumn() + tf.getText().length(), tf.getRow());
				cout << "| ";
				if (tf.getText().length() == 0) {
					lt.setText(_getRecent());
					lt.showList(0);
				}
			}
		}
	}
}
void openFile(TextField tf,int action, Index &index, SearchResult rs) {
	system("CLS");
	View w;
	w.setTextColor(GREEN);
	cout << "Press ENTER or BACKSPACE to back..." << endl << endl;
	ifstream file;
	file.open(index.getFilePath(rs.fileIndex));
	w.setTextColor(GREEN);
	cout << "FILE PATH: ";
	w.setTextColor(YELLOW);
	cout << index.getFilePath(rs.fileIndex) << endl;
	w.setTextColor(GREEN);
	cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
	string temp;
	int length = 0;
	getline(file, temp);
	w.setTextColor(LIGHTBLUE);
	cout << temp << endl << endl;
	w.setTextColor(255);
	while (getline(file, temp)) {
		cout << temp << endl;
		if (temp.length() == 0) cout << endl;
		//cout << temp << " ";
	}
	w.cursorPosition(0, 0);
	int t;
	while (true) {
		t = _getch();
		if (t == 13 || t == '\b') {
			system("CLS");
			//_getch();
			return;
	}
	}
}
void activity2(string text, Index &index, Synonym &synonym) {
	system("CLS");
	ListResult lr;
	TextField tf;
	vector<SearchResult> results;
	tf.setColumn(1);
	tf.setMaxChar(95);
	tf.setRow(2);	
	tf.sketch(LIGHTBLUE);
	tf.setText(text);
	tf.show(YELLOW);
	string ttext = "";
	int a;
	int action = 0;
	while (true) {
		if (ttext != tf.getText()) {
			tf.cursorPosition(99, 2); cout << "Processing...";
			results = processQuery(index, synonym, tf.getText());
			ttext = tf.getText();
			lr.getList(results);
			lr.sketch(tf.getText(), 0, index);
			action = 0;
		}
		
		if (results.size() != 0) {
			lr.draw(action);
			do {
				a = _getch();
				if (a == 224) {
					int b = _getch();
					if (b == 72) {
						int lrl;
						lrl = lr.getSize();
						if (action == 0) action = lrl;
						action = abs((action - 1) % lrl);
						lr.draw(action);
					}
					if (b == 80) {
						int lrl;
						lrl = lr.getSize();
						action = (action + 1) % lrl;
						lr.draw(action);
					}
				}
				else
					if (a == 13) {
						openFile(tf,action, index, results[action]);
						_getch();
						//input(tf, YELLOW);
						lr.sketch(tf.getText(), 0, index);
						lr.draw(action);
					}
					else {
						_getch();
					}
			} while (a == 224 || a == 13);
		}
		input(tf, YELLOW);
	}
}
