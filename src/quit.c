#include <curses.h>

#include "global.h"
#include "quit.h"
#include "uibase.h"

void quit_tab_transition() {
    if(menu_tab_focus == QUIT_TAB)
        return;
    
    erase();
    refresh();

    // to keep backup safe
    if(menu_tab_focus_backup[1] != QUIT_TAB)
        menu_tab_focus_backup[0] = menu_tab_focus;
    menu_tab_focus = QUIT_TAB;
    menu_tab_update();
    wrefresh(menu_tab);

    quit_tab_update();
    wrefresh(contents);
}

void quit_tab_update() {
    int row_pos = win_row / 2 - 2;
    mvwaddstr(contents, row_pos, win_col / 2 - 10, "Really want to Quit?");
    int col_pos = win_col / 2 - 3;
    row_pos++;
    mvwaddch(contents, row_pos, col_pos++, '[');
    wattron(contents, A_UNDERLINE);
    mvwaddch(contents, row_pos, col_pos++, 'Y');
    wattroff(contents, A_UNDERLINE);
    mvwaddstr(contents, row_pos, col_pos, "es]");
}