#include <curses.h>

#include "global.h"
#include "manual.h"
#include "uibase.h"

enum ManualPage manual_page_focus;

char manual_page_names[MAN_PAGE_CNT][MAN_PAGE_NAME_LEN] = {"Intro", "Code ", "File ", "Build"};

void manual_tab_transition() {
    if(menu_tab_focus == MANUAL_TAB)
        return;

    erase();
    refresh();

    menu_tab_focus = MANUAL_TAB;
    menu_tab_update();
    wrefresh(menu_tab);

    // todo : show MANUAL_TAB
    manual_page_print();
}

void manual_page_print() {
    wclear(opened_file_tab);
    wrefresh(opened_file_tab);

    // print page labels
    int col = 0;
    int len = win_col / MAN_PAGE_CNT;
    int front_blank = (len - MAN_PAGE_NAME_LEN - 2) / 2;
    for(int i = 0; i < MAN_PAGE_CNT; i++) {
        mvwaddch(opened_file_tab, 0, col, '\\');
        wattron(opened_file_tab, A_UNDERLINE);
        if(i == manual_page_focus)
            wattron(opened_file_tab, A_STANDOUT);
        for(int j = 0; j < front_blank; j++)
            mvwaddch(opened_file_tab, 0, col + 1 + j, ' ');
        mvwprintw(opened_file_tab, 0, col + 1 + front_blank, "%s", manual_page_names[i]);
        for(int j = front_blank + MAN_PAGE_NAME_LEN; j < len; j++)
            mvwaddch(opened_file_tab, 0, col + j, ' ');
        wattroff(opened_file_tab, A_UNDERLINE | A_STANDOUT);
        col += len;
        mvwaddch(opened_file_tab, 0, col - 1, '/');
    }
    wrefresh(opened_file_tab);

    // print contents - manual page
    wclear(contents);
    switch (manual_page_focus)
    {
    case INTRO_MAN:
        mvwprintw(contents, 0, 0, "contents");
        break;
    case FILE_MAN:
        break;
    case CODE_MAN:
        break;
    case BUILD_MAN:
        break;
    default:
        perror("manual_page_print() : page out of index");
        break;
    }
    wrefresh(contents);
}