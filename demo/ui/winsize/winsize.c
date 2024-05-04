#include <stdio.h>
#include <unistd.h>
#include <curses.h>
#include <signal.h>
#include <sys/ioctl.h>

#define WIN_MIN_ROW 30
#define WIN_MIN_COL 80

int win_row;
int win_col;

void print_winsize() {
	struct winsize winattr;
	ioctl(STDIN_FILENO, TIOCGWINSZ, &winattr);
	win_row = winattr.ws_row;
	win_col = winattr.ws_col;

	if(win_row < WIN_MIN_ROW || win_col < WIN_MIN_COL) {
		mvprintw(win_row / 2, win_col / 2 - 15, "Terminal size is too small");
	}
	else {
		mvprintw(win_row / 2, win_col / 2 - 15, "Terminal Size : %d x %d ", win_row, win_col); // Notice : LINES & COLS does not changes
	}
	// Notice : Change window size will break all printed layout - need to print all item & refresh & let message print at first line
	refresh();
}

void wininch_handler(int signum) {
	mvprintw(1, 0, "sig");
	print_winsize();	
}

int main() {
	int ch;
	signal(SIGWINCH, wininch_handler);

	initscr();
	raw();
	noecho();
	curs_set(0);
	print_winsize();

	mvprintw(0, 0, "q to quit\n");
	while (1) {
		ch = getch();
		if(ch == 'q')
			break;
	}

	endwin();

	return 0;
}
// IDEA : you need to make method that print each element of screen (init page also works in same manner too)
// this will prevent screen chars gonna break
