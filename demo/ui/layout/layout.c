#include <stdio.h>
#include <unistd.h>
#include <panel.h>
#include <stdlib.h>

#define CTRL(c) ((c) & 037)  // macro function to read CTRL + character
#define DEBUG_INPUT

#define KEY_NAME_WIDTH 14
#define FILE_TAB_WIDTH 15
#define MIN_MENU_TAB_WIDTH 10
#define MENU_TAB_CNT 6
#define MIN_WIDTH (MIN_MENU_TAB_WIDTH * MENU_TAB_CNT)

#define CODE_TAB 0
#define FILE_TAB 1
#define COMP_TAB 2
#define CMND_TAB 3
#define MANL_TAB 4
#define QUIT_TAB 5

typedef struct {
	char *file_name;
	char *full_path;
} TabContents;

// todo : newFileTab append manage dynamic list of openned files
// delFileTab will delete it & winsize change let max_file_tab changed, reallocation will needed

int max_file_tab; // cur window size determine this
int file_tab_cnt;
int file_tab_focus;

void newFileTab(WINDOW *head_tab) {
	int start_pos = 0;
	if(file_tab_cnt >= max_file_tab) {
		// todo : delete tab by policy
	} else
		file_tab_cnt++;
	// calculate start_pos
	start_pos = KEY_NAME_WIDTH + FILE_TAB_WIDTH * (file_tab_cnt - 1); 
		
	// print tab
	mvwprintw(head_tab, 0, start_pos, "\\");
	wattron(head_tab, A_UNDERLINE);
	mvwprintw(head_tab, 0, start_pos+1, "%-*s", FILE_TAB_WIDTH - 2, "file1.c");
	wattroff(head_tab, A_UNDERLINE);
	mvwprintw(head_tab, 0, start_pos+FILE_TAB_WIDTH-1, "/");

	wrefresh(head_tab);
}

void delFileTab() {
	// if delTab == foucs -> change focus
}

// todo!!! : change tab name from head, tail to file, menu
void tailTabPrint(WINDOW *tail_tab, int max_col, int focus) {
	if(max_col < MIN_WIDTH) {
		perror("too small window size\n");
		exit(1);
	}
	// todo : menu tab size calcul when winsize changed
	int width = COLS / MENU_TAB_CNT;

	// print
	for(int i = 0; i < MENU_TAB_CNT; i++) {
		int startpos = width * i;
		mvwaddch(tail_tab, 0, startpos, ' ');
		mvwaddch(tail_tab, 1, startpos, '/');
		for(int j = 0; j < width - 1; j++)
			mvwaddch(tail_tab, 0, startpos + j + 1 , '_');
		mvwaddch(tail_tab, 0, startpos + width - 1, ' ');
		mvwaddch(tail_tab, 1, startpos + width - 1, '\\');
	}

	wrefresh(tail_tab);

}

int main() {
	int input_char;
	max_file_tab = 5, file_tab_cnt = 0, file_tab_focus = -1;

	// init
	initscr();
	raw(); // line buffering disabled
	keypad(stdscr, true); // enables function key read
	noecho();
	curs_set(0); // hide cursor
	refresh();

	// todo : import termios, replace LINES & COLS
	// declare windows
	WINDOW *contents = newwin(LINES - 3, COLS, 1, 0);
	WINDOW *head_tab = newwin(1, COLS, 0, 0);
	WINDOW *tail_tab = newwin(2, COLS, LINES - 2, 0);

	// head tab init
	wattron(head_tab, A_UNDERLINE);
	mvwprintw(head_tab, 0, 0, "KEY_NAME     ");
	wattroff(head_tab, A_UNDERLINE);
	mvwprintw(head_tab, 0, KEY_NAME_WIDTH - 1, "/");
	wrefresh(head_tab);

	// tail tab init
	tailTabPrint(tail_tab, COLS, CODE_TAB);

	// test code
	int strow, stcol, maxrow, maxcol;
	getbegyx(contents, strow, stcol);
	getmaxyx(contents, maxrow, maxcol);
	mvwprintw(contents, 3, 0, "row : %d ~ %d  col : %d ~ %d", strow, maxrow, stcol, maxcol);
	mvwprintw(contents, 4, 0, "LINES : %d  COLS : %d", LINES, COLS);
	mvwprintw(contents, maxrow, 0, "maxrow");
	mvwprintw(contents, maxrow - 1, 0, "maxrow - 1");
	wrefresh(contents);

	// print
	mvwprintw(contents, 0, 0, "Press Ctrl + q to Quit");
	wrefresh(contents);

	// input handling
	while(1) {
		input_char = getch();
#ifdef DEBUG_INPUT
		// print input keyname
		wattron(head_tab, A_UNDERLINE);
		mvwprintw(head_tab, 0, 0, "%-*s", KEY_NAME_WIDTH - 1, keyname(input_char));
		wattroff(head_tab, A_UNDERLINE);
		wrefresh(head_tab);
#endif
		if (input_char == CTRL('q'))
			break;
		else if (input_char == 'n') {
			newFileTab(head_tab);
		}
		else {
			mvwprintw(contents, 1, 0, "Input : %s\nHex : 0x%x", keyname(input_char), input_char);
			wrefresh(contents);
		}
	}

	// terminate
	endwin();

	return 0;
}
