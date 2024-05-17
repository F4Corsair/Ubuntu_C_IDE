#include <curses.h>
#include <stdlib.h>

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
    opened_file_tab_update();
    wrefresh(opened_file_tab);
}

void opened_file_tab_update() {

}

void new_opened_file_tab(char *file_name, char *full_path) {


    // memory free
    free(file_name);
    free(full_path);
}

void del_opened_file_tab(int idx) {

}

OpenedFileInfo *opened_file_info_init() {
    OpenedFileInfo *info = malloc(sizeof(OpenedFileInfo));
    info->cnt = 0;
    info->focus = -1;
    info->head = NULL;

    return info;
}

void opened_file_info_terminate(OpenedFileInfo *info) {
    FileStatus *ptr = info->head;
    if(ptr != NULL) {
        FileStatus *next;
        while(ptr != NULL) {
            next = ptr->next;
            free(ptr);
            ptr = next;
        }
    }
    info->cnt = info->focus = 0;
}