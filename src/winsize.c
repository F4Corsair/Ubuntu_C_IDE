#include <stdio.h>
#include <curses.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>

#include "winsize.h"
#include "uibase.h"

void wininch_handler(int signum) {
	mvprintw(1, 0, "sig");
	
    // todo : pause system
    winsize_calculate();
    // todo : clear window - use method defined on uibase.h

   	if(win_row < WIN_MIN_ROW || win_col < WIN_MIN_COL) {
        // show msg : Terminal size is too small!
    }
    else {
        // show msg : Window size change detected
        // if pre-process is done : ask user to continue
    }
    refresh(); // atomic implement?
}

void winsize_init() {
    signal(SIGWINCH, wininch_handler);
}

void winsize_calculate() {
    struct winsize winattr;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &winattr);
    win_row = winattr.ws_row;
    win_col = winattr.ws_col;
}