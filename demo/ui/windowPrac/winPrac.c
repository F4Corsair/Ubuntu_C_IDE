#include <stdio.h>
#include <curses.h>
#include <unistd.h>

int main() {
	initscr();
	keypad(stdscr, TRUE);
	box(stdscr, '|', '-');
	mvprintw(LINES / 2, COLS / 2 - 10, "from stdscr");
	// new window overlap printed data
	refresh();

	WINDOW *test = newwin(LINES - 10, COLS - 10, 5, 5);
	box(test, '|', '*');
	// box overwrites all edges & its inner contents as blank
	mvwprintw(test ,0, 0, "from newwin");
	// print position is relative to WINDOW
	wrefresh(test);

	mvprintw(LINES / 2, COLS / 2 - 10, "from stdscr");
	refresh();
	// window doesn't have its precedence or level. only has its position to write

	getch();

	delwin(test);
	endwin();
	return 0;
}
