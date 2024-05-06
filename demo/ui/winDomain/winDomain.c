#include <stdio.h>
#include <unistd.h>
#include <panel.h>

#define CTRL(c) ((c) & 037)  // macro function to read CTRL + character

int main() {
	int input_char;

	// init
	initscr();
	raw(); // line buffering disabled
	keypad(stdscr, true); // enables function key read
	noecho();
	curs_set(0); // hide cursor
	refresh();


	WINDOW *contents = newwin(LINES - 1, COLS, 1, 0);
	wrefresh(contents);

	// test code
	int strow, stcol, maxrow, maxcol;
	getbegyx(contents, strow, stcol);
	getmaxyx(contents, maxrow, maxcol);
	mvwprintw(contents, 3, 0, "row : %d ~ %d  col : %d ~ %d", strow, maxrow, stcol, maxcol);
	mvwprintw(contents, 4, 0, "LINES : %d  COLS : %d", LINES, COLS);
	mvwprintw(contents, maxrow, 0, "maxrow");
	mvwprintw(contents, maxrow - 1, 0, "maxrow - 1");
	wrefresh(contents);

	// notice : row & col domain is [0, LINES - 1] & [0, COLS - 1]


	// print
	mvwprintw(contents, 0, 0, "Press Ctrl + q to Quit");
	wrefresh(contents);

	// input handling
	while(1) {
		input_char = getch();
		if (input_char == CTRL('q'))
			break;
		else {
			mvwprintw(contents, 1, 0, "Input : %s\nHex : 0x%x", keyname(input_char), input_char); // you don't need to refresh with this method
			wrefresh(contents);
		}
	}

	// terminate
	endwin();

	return 0;
}
