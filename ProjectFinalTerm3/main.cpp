#include "headers/gui.h"
#include "headers/search_activity.h"
int main() {
	View w;
	w.ShowConsoleCursor(false);
	w.drawTitle();
	TextField tf;
	tf.setColumn(10);
	tf.setMaxChar(95);
	tf.setRow(7);
	tf.sketch(RED);
	input(tf, YELLOW);
	system("pause");
	return 0;
}