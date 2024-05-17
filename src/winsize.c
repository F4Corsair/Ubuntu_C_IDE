#include <stdio.h>
#include <curses.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>

#include "winsize.h"
#include "uibase.h"

void wininch_handler(int signum) {
    if(menu_tab_focus != WINSIZE_TAB)
        menu_tab_focus_backup[1] = menu_tab_focus;
    menu_tab_focus = WINSIZE_TAB;
    winsize_flag = 1; // this flag blocks keyboard input
	
    // todo : pause system
    winsize_calculate();

    erase();
    refresh();

    int row_pos = win_row / 2 - 1;
    row_pos = row_pos < 0 ? 0 : row_pos;
    int col_pos = win_col / 2 - 14;
    col_pos = col_pos < 0 ? 0 : col_pos;
   	if(win_row < WIN_MIN_HEIGHT || win_col < WIN_MIN_WIDTH) {
        mvaddstr(row_pos, col_pos, "Window size change detected");
        mvaddstr(row_pos + 1, col_pos, "Terminal Size is too small!");
        refresh();
    }
    else {
        mvaddstr(row_pos, col_pos, "Window size change detected");
        refresh();
        // if pre-process is done : ask user to continue
        mvaddstr(row_pos + 1, col_pos, "[Press any key to Continue]");
        refresh();
        winsize_flag = 2;
    }
}

void winsize_init() {
    signal(SIGWINCH, wininch_handler);
    winsize_calculate();

    if(win_row < WIN_MIN_HEIGHT || win_col < WIN_MIN_WIDTH) {
        winsize_flag = 1; // to block key input
        // show msg : Terminal size is too small!
    } else {
        winsize_flag = 0;
    }
}

void winsize_calculate() {
    struct winsize winattr;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &winattr);
    win_row = winattr.ws_row;
    win_col = winattr.ws_col;
}