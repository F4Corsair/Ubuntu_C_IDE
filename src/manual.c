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
        mvwprintw(contents, 0, 0, "You can switch manual page with arrow key (<- and ->)");
        mvwprintw(contents, 2, 0, "To switch Menu Tab, try CTRL + Underlined Char");
        mvwprintw(contents, 3, 0, "Code Tab : Ctrl + c");
        mvwprintw(contents, 4, 0, "File Tab : Ctrl + f");
        mvwprintw(contents, 5, 0, "Build Tab : Ctrl + b");
        mvwprintw(contents, 6, 0, "Terminal Tab : Ctrl + t");
        mvwprintw(contents, 7, 0, "Help Tab (Manual) : Ctrl + h");
        mvwprintw(contents, 9, 0, "If you want to quit program, press Ctrl + q");
        mvwprintw(contents, 10, 0, "after that, press y or Y will Quit, else will let you back");
        mvwprintw(contents, 12, 0, "This program supports Dynamic window size");
        mvwprintw(contents, 13, 0, "If you resized terminal, just press any key input to restore program");

        break;
    case FILE_MAN:
        mvwprintw(contents, 0, 0, "Notice : This program can read ASCII characters only - be careful when you select file to open");
        mvwprintw(contents, 2, 0, "To navigate directories:");
        mvwprintw(contents, 3, 0, "Press the right arrow key to move down into sub-directory");
        mvwprintw(contents, 4, 0, "Press the left arrow key to move parent-directory");
        mvwprintw(contents, 5, 0, "If you press the right key in a directory without subdirectories, a warning message appears");
        mvwprintw(contents, 6, 0, "Press Enter key to set focused directory as workspace directory");
        mvwprintw(contents, 7, 0, "If you press Enter key in a directory without subfiles, a warning message will appear");
        mvwprintw(contents, 8, 0, "Press 'r' to change workspace - it will reset & turn back to home dir");
        mvwprintw(contents, 9, 0, "If you press the 'c' key in the file tab, you can open the file in the code tab");
        break;
    case CODE_MAN:
        mvwprintw(contents, 0, 0, "You can handle Opened File Tab (You need to open file from File Tab)");
        mvwprintw(contents, 1, 0, "Change file tab focus : Ctrl + PGDN / Ctrl + PGUP");
        mvwprintw(contents, 2, 0, "Delete focused file tab : Ctrl + w");
        mvwprintw(contents, 3, 0, "If file unsaved, program will ask how to handle it : press underlined character");
        mvwprintw(contents, 4, 0, "Save current file : Ctrl + s");
        mvwprintw(contents, 6, 0, "You can move file cursor using Arrow & PageUp & PageDown key");
        mvwprintw(contents, 7, 0, "You can edit files with key input but only ascii defined character are allowed");
        mvwprintw(contents, 8, 0, "Alt + up/down arrow can let you swap 2 code line");
        break;
    case BUILD_MAN:
        mvwprintw(contents, 0, 0, "if makefile exist on workspace directory, it will excute");
        break;
    default:
        perror("manual_page_print() : page out of index");
        break;
    }
    wrefresh(contents);
}