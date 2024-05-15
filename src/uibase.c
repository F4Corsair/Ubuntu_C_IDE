#include <stdio.h>
#include <unistd.h>
#include <panel.h>
#include <signal.h>

#include "uibase.h"
#include "code.h"
#include "quit.h"

int file_tab_cnt; // number of current file tab
int file_tab_focus;

WINDOW *file_tab;
WINDOW *menu_tab;
WINDOW *contents;

void ui_init() {
    menu_tab_focus = CODE_TAB;
    
    // set base property
    initscr();
    raw(); // line buffering disabled
    keypad(stdscr, true); // enables to read dynamic key input
    noecho();
    curs_set(0); // hide cursor
    refresh();

    // declare signal to ignore
    signal(SIGINT, SIG_IGN);

    // notice : print ui to stdscr must not work here
}

void ui_terminate() {
    delwin(menu_tab);
    delwin(file_tab);
    delwin(contents);
    endwin();
}

void ui_set_whole() {
    menu_tab_reset();
    wrefresh(menu_tab);

    file_tab = newwin(1, win_col, 0 , 0);

    contents = newwin(win_row - 3, win_col, 1, 0);
}

void menu_tab_reset() {
    delwin(menu_tab);
    menu_tab = newwin(2, win_col, win_row - 2, 0);
    menu_tab_update();
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
		wattron(menu_tab, A_UNDERLINE);
		mvwaddch(menu_tab, 1, startpos + (tab_width - 8) / 2 + menu_tab_pos[i] , menu_tab_names[i][menu_tab_pos[i]]);
		wattroff(menu_tab, A_UNDERLINE | A_STANDOUT);
    }
}

void tab_restore() {
    if(menu_tab_focus != QUIT_TAB && menu_tab_focus != WINSIZE_TAB) {
        perror("Wrong Call : tab_restore()");
        return;
    }

    switch (menu_tab_focus_backup)
    {
    case CODE_TAB:
        code_tab_transition();
        break;
    case FILE_TAB:
        break;
    case BUILD_TAB:
        break;
    case TERMINAL_TAB:
        break;
    case MANUAL_TAB:
        break;
    case QUIT_TAB:
        quit_tab_transition();
        break;
    }

    menu_tab_update();

    return;
}