#include <panel.h>

#include "global.h"
#include "code.h"
#include "uibase.h"

void code_tab_transition() {
    if(menu_tab_focus == CODE_TAB)
        return;

    erase();
    refresh();

    menu_tab_focus = CODE_TAB;
    menu_tab_update();
    wrefresh(menu_tab);

    // todo : show CODE_TAB
}