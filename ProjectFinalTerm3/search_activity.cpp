#include "headers/search_activity.h"
void input(TextField tf, int color) {
	ListText lt;
	int listLength = 1;
	int act = 0;
	lt.setup(tf);
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
		else if (b == 13) {
			_getch();
			if (act == 0) {
				logFile(tf.getText());
			}
			else {
				logFile(lt.getText(act - 1));
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
			}
		}
	}
}
