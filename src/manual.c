#include <curses.h>

#include "global.h"
#include "manual.h"
#include "uibase.h"

void manual_tab_transition() {
    if(menu_tab_focus == MANUAL_TAB)
        return;

    erase();
    refresh();

    menu_tab_focus = MANUAL_TAB;
    menu_tab_update();
    wrefresh(menu_tab);

    // todo : show MANUAL_TAB
}