#include <stdio.h>
#include <unistd.h>
#include <panel.h>
#include <signal.h>

#include "uibase.h"

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
    // delwin
    endwin();
}