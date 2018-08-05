#ifndef _search_activity_h_
#define _search_activity_h_

#include <iostream>
#include "gui.h"
#include "history.h"
#include <conio.h>
#include "Index.h"
#include "Synonym.h"
#include "Searcher.h"

using namespace std;

void input(TextField &tf, int color);
void activity2(string text, Index &index, Synonym &synonym);
#endif
