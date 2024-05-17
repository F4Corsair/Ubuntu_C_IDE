#include <stdio.h>
#include <unistd.h>
#include <panel.h>
#include <stdlib.h>

#define CTRL(c) ((c) & 037)  // macro function to read CTRL + character
#define DEBUG_INPUT // debug option : print input charcter

#define KEY_NAME_WIDTH 14
#define FILE_TAB_WIDTH 15
#define MIN_MENU_TAB_WIDTH 10
#define MENU_TAB_CNT 6
#define MIN_WIDTH (MIN_MENU_TAB_WIDTH * MENU_TAB_CNT)

#define CODE_TAB 0
#define FILE_TAB 1
#define BUILD_TAB 2
#define TERMINAL_TAB 3
#define MANUAL_TAB 4
#define QUIT_TAB 5

typedef struct {
	char *file_name;
	char *full_path;
} TabContents;

char menu_tab_names[MENU_TAB_CNT][MIN_MENU_TAB_WIDTH - 1] = {"  Code  ", "  File  ", "  Build ", "Terminal", " Manual ", "  Quit  "};
int menu_tab_pos[MENU_TAB_CNT] = {2, 2, 2, 0, 1, 2}; 

// todo : newFileTab append manage dynamic list of openned files
// delFileTab will delete it & winsize change let max_file_tab changed, reallocation will needed

int max_file_tab; // cur window size determine this
int file_tab_cnt;
int file_tab_focus;

void newFileTab(WINDOW *file_tab) {
	int start_pos = 0;
	if(file_tab_cnt >= max_file_tab) {
		// todo : delete tab by policy
	} else
		file_tab_cnt++;
	// calculate start_pos
	start_pos = KEY_NAME_WIDTH + FILE_TAB_WIDTH * (file_tab_cnt - 1); 
		
	// print tab
	mvwprintw(file_tab, 0, start_pos, "\\");
	wattron(file_tab, A_UNDERLINE);
	mvwprintw(file_tab, 0, start_pos+1, "%-*s", FILE_TAB_WIDTH - 2, "file1.c");
	wattroff(file_tab, A_UNDERLINE);
	mvwprintw(file_tab, 0, start_pos+FILE_TAB_WIDTH-1, "/");

	wrefresh(file_tab);
}

void delFileTab() {
	// if delTab == foucs -> change focus
}

// todo!!! : change tab name from head, tail to file, menu
void menuTabPrint(WINDOW *menu_tab, int max_col, int focus) {
	if(max_col < MIN_WIDTH) {
		perror("too small window size\n");
		exit(1);
	}
	// todo : menu tab size calcul when winsize changed
	int width = COLS / MENU_TAB_CNT;

	// print
	for(int i = 0; i < MENU_TAB_CNT; i++) {
		int startpos = width * i;
		mvwaddch(menu_tab, 0, startpos, ' ');
		for(int j = 0; j < width - 1; j++)
			mvwaddch(menu_tab, 0, startpos + j + 1 , '_');
		mvwaddch(menu_tab, 0, startpos + width - 1, ' ');

		mvwaddch(menu_tab, 1, startpos, '/');
		mvwaddch(menu_tab, 1, startpos + width - 1, '\\');

		if(i == focus)
			wattron(menu_tab, A_STANDOUT);
		mvwprintw(menu_tab, 1, startpos + 1, "%*s", width - 2, "");
		mvwprintw(menu_tab, 1, startpos + (width - 8) / 2, "%s", menu_tab_names[i]);
		wattron(menu_tab, A_UNDERLINE);
		mvwaddch(menu_tab, 1, startpos + (width - 8) / 2 + menu_tab_pos[i] , menu_tab_names[i][menu_tab_pos[i]]);
		wattroff(menu_tab, A_UNDERLINE | A_STANDOUT);
	}

	wrefresh(menu_tab);

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
	WINDOW *file_tab = newwin(1, COLS, 0, 0);
	WINDOW *menu_tab = newwin(2, COLS, LINES - 2, 0);

	// file tab init
	wattron(file_tab, A_UNDERLINE);
	mvwprintw(file_tab, 0, 0, "KEY_NAME     ");
	wattroff(file_tab, A_UNDERLINE);
	mvwprintw(file_tab, 0, KEY_NAME_WIDTH - 1, "/");
	wrefresh(file_tab);

	// menu tab init
	menuTabPrint(menu_tab, COLS, CODE_TAB);

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
		wattron(file_tab, A_UNDERLINE);
		mvwprintw(file_tab, 0, 0, "%-*s", KEY_NAME_WIDTH - 1, keyname(input_char));
		wattroff(file_tab, A_UNDERLINE);
		wrefresh(file_tab);
#endif
		if (input_char == CTRL('q'))
			break;
		else if (input_char == 'n') {
			newFileTab(file_tab);
		}
		else {
			mvwprintw(contents, 1, 0, "Input : %s\nHex : 0x%x", keyname(input_char), input_char);
			wrefresh(contents);
		}
	}

	// terminate
	delwin(file_tab);
	delwin(menu_tab);
	endwin();

	return 0;
}

/*
 * if winsize changed & catched its signal, raise win_size_changed flag and display alert page first (flag will stop all input handling)
 * then file saving procedure will call - if finished, [press any key to refresh] message will display
 * any key input will load all window from scratch but reference working data
*/