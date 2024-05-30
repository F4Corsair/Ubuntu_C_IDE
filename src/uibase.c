#include <stdio.h>
#include <unistd.h>
#include <curses.h>
#include <signal.h>
#include <stdlib.h>

#include "global.h"
#include "uibase.h"
#include "code.h"
#include "file.h"
#include "build.h"
#include "terminal.h"
#include "manual.h"
#include "quit.h"

OpenFileInfo *opened_file_info;

WINDOW *opened_file_tab;
WINDOW *menu_tab;
WINDOW *contents;

void ui_init() {
    opened_file_info = opened_file_info_init();
    
    // set base property
    initscr();
    raw(); // line buffering disabled
    keypad(stdscr, true); // enables to read dynamic key input
    noecho();
    curs_set(0); // hide cursor

    // color check
    if(has_colors() == FALSE) {
        endwin();
        printf("Terminal does not support color");
        perror("Terminal does not support color");
        exit(1);
    }
    start_color();

    // COLOR DEFINITION
    init_pair(2, COLOR_GREEN, COLOR_BLACK);
    init_pair(3, COLOR_RED, COLOR_BLACK);
    init_pair(4, COLOR_BLUE, COLOR_BLACK);

    refresh();

    // declare signal to ignore
    signal(SIGINT, SIG_IGN);

    // notice : print ui to stdscr must not work here
}

void ui_terminate() {
    opened_file_info_terminate();
    delwin(menu_tab);
    delwin(opened_file_tab);
    delwin(contents);
    endwin();
}

void ui_set_whole() {
    window_reset();
    menu_tab_focus = FILE_TAB;
    manual_page_focus = INTRO_MAN;
    unsaved_caution_flag = 0;
    code_tab_transition();
    menu_tab_update();
    wrefresh(menu_tab);
}

void window_reset() {
    delwin(menu_tab);
    delwin(opened_file_tab);
    delwin(contents);
    // NOTICE : you have to call 'endwin()' before reset window size!!!
    endwin();
    refresh();

    menu_tab = newwin(2, win_col, win_row - 2, 0);
    opened_file_tab = newwin(1, win_col, 0 , 0);
    contents = newwin(win_row - 3, win_col, 1, 0);
}

void menu_tab_update() {
    int tab_width = win_col / MENU_TAB_CNT;

    for(int i = 0; i < MENU_TAB_CNT; i++) {
        int startpos = tab_width * i;
        mvwaddch(menu_tab, 0, startpos, ' ');
        for(int j = 0; j < tab_width - 1; j++)
			mvwaddch(menu_tab, 0, startpos + j + 1 , '_');
        mvwaddch(menu_tab, 0, startpos + tab_width - 1, ' ');

		mvwaddch(menu_tab, 1, startpos, '/');
		mvwaddch(menu_tab, 1, startpos + tab_width - 1, '\\');
        
        if(i == menu_tab_focus)
			wattron(menu_tab, A_STANDOUT);
		mvwprintw(menu_tab, 1, startpos + 1, "%*s", tab_width - 2, "");
		mvwprintw(menu_tab, 1, startpos + (tab_width - 8) / 2, "%s", menu_tab_names[i]);
		wattroff(menu_tab, A_STANDOUT);
    }
}

void tab_restore() {
    enum MenuTab focus;
    if (menu_tab_focus == QUIT_TAB) {
        focus = menu_tab_focus_backup[0];
    } else if(menu_tab_focus == WINSIZE_TAB) {
        focus = menu_tab_focus_backup[1];
    } else {
        perror("Wrong Call : tab_restore()");
        return;
    }

    switch (focus)
    {
    case CODE_TAB:
        code_tab_transition();
        return;
    case FILE_TAB:
        file_tab_transition();
        return;
    case BUILD_TAB:
        build_tab_transition();
        return;
    case TERMINAL_TAB:
        terminal_tab_transition();
        return;
    case MANUAL_TAB:
        manual_tab_transition();
        return;
    case QUIT_TAB:
        quit_tab_transition();
        return;
    }
}