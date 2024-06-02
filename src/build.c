#include <curses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "global.h"
#include "build.h"
#include "uibase.h"
#include "file.h"

#define PIPE_ENDS 2

typedef struct compile {
	char line[BUFSIZ];
	struct results* next;
} results;

void build_tab_transition() {
    if(menu_tab_focus == BUILD_TAB)
        return;

    erase();
    refresh();

    menu_tab_focus = BUILD_TAB;
    menu_tab_update();
    wrefresh(menu_tab);

    // show BUILD_TAB
    opened_workspace_tab_print();
    build_compile_print();
}

void build_compile_print() {
    int read_len;
    char buf[BUFSIZ];
    FILE *pipe_fp;

    if (opened_file_info == NULL) {
        fprintf(stderr, "Error: opened_file_info is NULL\n");
        return;
    }

    wclear(contents);
    results *compile_result = (results*)malloc(sizeof(results));

    pipe_fp = popen("make", "r"); // "make" 명령어를 실행하고 해당 출력을 읽을 파일 스트림을 생성
    if (pipe_fp == NULL) {
        perror("popen");
        exit(1);
    }

    int row = 1;
    mvwprintw(contents, row++, 0, "compile results:");
    while (fgets(buf, BUFSIZ, pipe_fp) != NULL) { // 프로세스의 출력을 한 줄씩 읽어옴
        mvwprintw(contents, row++, 0, "\t%s", buf);
        if (row >= win_row - 1) {
            wclear(contents); // clear the window if it fills up
            row = 0;
        }
        wrefresh(contents); // refresh the window after each write
    }

    if (pclose(pipe_fp) == -1) { // 프로세스를 닫음
        perror("pclose");
        exit(1);
    }

    wrefresh(contents);
}