#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <curses.h>
#include "global.h"
#include "file.h"
#include "uibase.h"
#include "ls.h"

int max_file_tab; // 현재 창 크기에 따라 결정됨
int file_tab_cnt;
int file_tab_focus;

void file_tab_transition() {
    if (menu_tab_focus == FILE_TAB)
        return;

    erase();
    refresh();
    menu_tab_focus = FILE_TAB;
    menu_tab_update();
    wrefresh(menu_tab);

    initialize_colors();

    max_file_tab = 5;
    file_tab_cnt = 0;
    file_tab_focus = -1;

    file_contents = newwin(LINES - 4, COLS, 2, 0);
    file_path = newwin(1, COLS, 1, 0);
    file_tab = newwin(1, COLS, 0, 0);

    wattron(file_tab, A_UNDERLINE);
    mvwprintw(file_tab, 0, 0, "WorkSpace1    ");
    wattroff(file_tab, A_UNDERLINE);
    mvwprintw(file_tab, 0, KEY_NAME_WIDTH - 1, "/");
    wrefresh(file_tab);

    char current_path[256];
    if (getcwd(current_path, sizeof(current_path)) != NULL) {
        print_path(file_path, current_path);
        print_contents(file_contents, current_path);
    }

    attroff(COLOR_PAIR(1));
}



void initialize_colors() {
    start_color(); // 색상 사용을 위해 초기화
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    refresh();
}

void print_path(WINDOW *path_win, const char *path) {
    int width = COLS - 2; // 테두리를 위한 공간 확보
    wattron(path_win, A_UNDERLINE);
    mvwprintw(path_win, 0, 0, "Path:");
    mvwprintw(path_win, 0, 5, " %-*s", width, path); // "Path: " 공간 확보
    wattroff(path_win, A_UNDERLINE);
    wrefresh(path_win);
}

void print_contents(WINDOW *contents_win, const char *path) {
    FileStatus work_space_file;
    int contents_row = 1, contents_col = 1; // 파일 선택을 위한 커서
    if (getcwd(work_space_file.full_path, sizeof(work_space_file.full_path)) != NULL) {
        ls(contents_win, work_space_file.full_path);
        mvwchgat(contents_win, contents_row, contents_col, 5, A_BLINK, 0, NULL);
    }
    wrefresh(contents_win);
}


void pathContents() {
    file_contents = newwin(LINES - 4, COLS, 2, 0);
    file_path = newwin(1, COLS, 1, 0);
}

void newFileTab(WINDOW *file_tab) {
    int start_pos = 0;
    char file_name[FILE_TAB_WIDTH];

    if (file_tab_cnt >= max_file_tab) {
        // TODO: 정책에 따라 탭 삭제
    } else {
        file_tab_cnt++;
    }

    // 시작 위치 계산
    start_pos = KEY_NAME_WIDTH + FILE_TAB_WIDTH * (file_tab_cnt - 1);

    // 탭 출력
    snprintf(file_name, sizeof(file_name), "WorkSpace%d", file_tab_cnt + 1);
    mvwprintw(file_tab, 0, start_pos, "\\");
    wattron(file_tab, A_UNDERLINE);
    mvwprintw(file_tab, 0, start_pos + 1, "%-*s", FILE_TAB_WIDTH - 2, file_name);
    wattroff(file_tab, A_UNDERLINE);
    mvwprintw(file_tab, 0, start_pos + FILE_TAB_WIDTH - 1, "/");

    wrefresh(file_tab);
}
