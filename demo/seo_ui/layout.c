#include <stdio.h>
#include <unistd.h>
#include <panel.h>
#include <stdlib.h>
#include <limits.h>
#include <stddef.h>
#include "ls.h"
#include "global.h"
#include "layout.h"





extern WorkSpaceContents* head;



int max_file_tab; // cur window size determine this
int file_tab_cnt;
int file_tab_focus;


void displayDirectoryContents(WINDOW*, char* full_path); 
void fileOpen(char* file_name);
void printFile(char* file_name);

void newFileTab(WINDOW *file_tab) {
    int start_pos = 0;
    char file_name[FILE_TAB_WIDTH];
    if(file_tab_cnt >= max_file_tab) {
        // todo : delete tab by policy
    } else
        file_tab_cnt++;
    // calculate start_pos
    start_pos = KEY_NAME_WIDTH + FILE_TAB_WIDTH * (file_tab_cnt - 1); 
        
    // print tab
    snprintf(file_name, sizeof(file_name), "WorkSpace%d", file_tab_cnt+1);  
    mvwprintw(file_tab, 0, start_pos, "\\");
    wattron(file_tab, A_UNDERLINE);
    mvwprintw(file_tab, 0, start_pos+1, "%-*s", FILE_TAB_WIDTH - 2, file_name);
    wattroff(file_tab, A_UNDERLINE);
    mvwprintw(file_tab, 0, start_pos+FILE_TAB_WIDTH-1, "/");

    wrefresh(file_tab);
}

void delFileTab() {
    // if delTab == foucs -> change focus
}

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

void printPath(WINDOW *path_win, char *path) {
    int width = COLS - 2; // Adjust width to leave space for borders
    
    wattron(path_win,A_UNDERLINE);
    mvwprintw(path_win, 0, 0, "Path:");
    mvwprintw(path_win, 0, 5, " %-*s", width, path); // Adjust position to leave space for "Path: "
    wattroff(path_win,A_UNDERLINE);
    wrefresh(path_win);
}




int main() {
    int input_char;
    max_file_tab = 5, file_tab_cnt = 0, file_tab_focus = -1;
    FileStatus work_space_file;
    work_space_file.full_path = (char*)malloc(sizeof(char)*256);
    int contents_row = 1, contents_col = 1; // cursor for file selection
    char contents_file_name[256]; int pid;
   
    // init
    initscr();
    raw(); // line buffering disabled
    keypad(stdscr, true); // enables function key read
    noecho();
    curs_set(0); // hide cursor
    start_color(); // 색상 사용을 위해 초기화
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    refresh();
    
    // declare windows
    file_contents = newwin(LINES - 4, COLS, 2, 0);
    file_path = newwin(1,COLS,1,0);
    file_tab = newwin(1, COLS, 0, 0);
    file_menu_tab = newwin(2, COLS, LINES - 2, 0);

    // file tab init
    wattron(file_tab, A_UNDERLINE);
    mvwprintw(file_tab, 0, 0, "WorkSpace1    ");
    wattroff(file_tab, A_UNDERLINE);
    mvwprintw(file_tab, 0, KEY_NAME_WIDTH - 1, "/");
    wrefresh(file_tab);

    // menu tab init
    menuTabPrint(file_menu_tab, COLS, FILE_TAB);

    // print path
    if (getcwd(work_space_file.full_path, 256) != NULL) {
	wprintw(file_path, "%s\n", work_space_file.full_path);
        printPath(file_path, work_space_file.full_path);
        work_space_file.file_name=".";
        displayDirectoryContents(file_contents, work_space_file.full_path);
	mvwchgat(file_contents, contents_row, contents_col, 5, A_BLINK, 0, NULL);
    } 
   
   wrefresh(file_contents);  
    // input handling
    while(1) {
        input_char = getch();
#ifdef DEBUG_INPUT

#endif
        if (input_char == CTRL('q'))
		break;
        else if (input_char == 'n') {
        	newFileTab(file_tab);
        }
	else if (input_char == KEY_DOWN) {
		wchgat(file_contents, -1, A_NORMAL, 0, NULL);
		contents_row++;
		mvwchgat(file_contents, contents_row, contents_col, 10, A_BLINK, 0, NULL);
		wrefresh(file_contents);
	}
	else if (input_char == KEY_UP) {
		wchgat(file_contents, -1, A_NORMAL, 0, NULL);
		contents_row--;
		mvwchgat(file_contents, contents_row, contents_col, 10, A_BLINK, 0, NULL);
		wrefresh(file_contents);
	}
    /*
	else if (input_char == '\n') { // file selection
	       wscanw(file_contents, "%s", contents_file_name);
	       if ((pid = fork()) < 0) {
		       perror("fork");
		       exit(1);
	       }
	       else if (pid == 0)
	       		printFile(contents_file_name);
	       else 
		       break;	
	}
    */
	    	
    }
    attroff(COLOR_PAIR(1));

    // terminate
    delwin(file_tab);
    delwin(file_menu_tab);
    endwin();

    return 0;
}