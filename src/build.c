#include <curses.h>

#include "global.h"
#include "build.h"
#include "uibase.h"

void build_tab_transition() {
    if(menu_tab_focus == BUILD_TAB)
        return;

    erase();
    refresh();

    menu_tab_focus = BUILD_TAB;
    menu_tab_update();
    wrefresh(menu_tab);

    // todo : show BUILD_TAB
}