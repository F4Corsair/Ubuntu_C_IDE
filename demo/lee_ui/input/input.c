#include <stdio.h>
#include <unistd.h>
#include <curses.h>

#define CTRL(c) ((c) & 037)  // macro function to read CTRL + character

int main() {
	int input_char; // you have to set read character type as integer to handle special inputs

	initscr();
	raw(); // line buffering disabled + ignore all signals
	keypad(stdscr, true); // enables function key read
	noecho();
	curs_set(0); // hide cursor
	mvprintw(0, 0, "Press Ctrl + ");
	addch('q' | A_BOLD);
	addstr(" to Quit");

	refresh();

	while(1) {
		input_char = getch();
		if (input_char == CTRL('q'))
			break;
		else {
			mvprintw(1, 0, "Input : %s\nHex : 0x%x", keyname(input_char), input_char); // you don't need to refresh with this method
		}
	}

	endwin();

	return 0;
}
