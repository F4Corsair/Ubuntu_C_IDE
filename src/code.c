#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "global.h"
#include "code.h"
#include "uibase.h"

void code_tab_transition() {
    if(menu_tab_focus == CODE_TAB)
        return;

    erase();
    refresh();

    unsaved_caution_flag = 0; // forget about unsaved handle procedure
    menu_tab_focus = CODE_TAB;
    menu_tab_update();
    wrefresh(menu_tab);

    // todo : show CODE_TAB
    opened_file_tab_print();
    wrefresh(opened_file_tab);
}
