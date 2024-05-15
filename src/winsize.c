#include <stdio.h>
#include <curses.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>

#include "winsize.h"
#include "uibase.h"

void wininch_handler(int signum) {
    menu_tab_focus_backup = menu_tab_focus;
    menu_tab_focus = WINSIZE_TAB;
    winsize_flag = 1; // this flag blocks keyboard input
	
    // todo : pause system
    winsize_calculate();
    // todo : clear window - use method defined on uibase.h

   	if(win_row < WIN_MIN_HEIGHT || win_col < WIN_MIN_WIDTH) {
        // show msg : Terminal size is too small!
    }
    else {
        // show msg : Window size change detected
        // if pre-process is done : ask user to continue
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