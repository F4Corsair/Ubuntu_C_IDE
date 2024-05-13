#include <stdio.h>
#include <curses.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ioctl.h>

#include "winsize.h"
#include "uibase.h"

void wininch_handler(int signum) {
    set_wait_any_input(TRUE); // todo : this flag blocks keyboard input
	
    // todo : pause system
    winsize_calculate();
    // todo : clear window - use method defined on uibase.h

   	if(win_row < WIN_MIN_HEIGHT || win_col < WIN_MIN_WIDTH) {
        // show msg : Terminal size is too small!
    }
    else {
        // show msg : Window size change detected
        // if pre-process is done : ask user to continue
        set_winsize_changed(FALSE);
        set_wait_any_input(TRUE);
    }
}

void winsize_init() {
    signal(SIGWINCH, wininch_handler);
    winsize_calculate();

    if(win_row < WIN_MIN_HEIGHT || win_col < WIN_MIN_WIDTH) {
        set_winsize_changed(TRUE); // to block key input
        // show msg : Terminal size is too small!
    } else {
        set_winsize_changed(FALSE);
    }
}

void winsize_calculate() {
    struct winsize winattr;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &winattr);
    win_row = winattr.ws_row;
    win_col = winattr.ws_col;
}