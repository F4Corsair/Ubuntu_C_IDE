#include <curses.h>

#include "global.h"
#include "file.h"
#include "uibase.h"

void file_tab_transition() {
    if(menu_tab_focus == FILE_TAB)
        return;

    erase();
    refresh();

    menu_tab_focus = FILE_TAB;
    menu_tab_update();
    wrefresh(menu_tab);

    // todo : show FILE_TAB
}