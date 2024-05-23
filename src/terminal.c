#include <curses.h>

#include "global.h"
#include "terminal.h"
#include "uibase.h"

void terminal_tab_transition() {
    if(menu_tab_focus == TERMINAL_TAB)
        return;

    erase();
    refresh();

    menu_tab_focus = TERMINAL_TAB;
    menu_tab_update();
    wrefresh(menu_tab);

    // todo : show TERMINAL_TAB
    wclear(contents);
    mvwprintw(contents, (win_row - 3)/ 2, win_col / 2 - 12, "[Terminal Tab is in dev]");
    wrefresh(contents);
}