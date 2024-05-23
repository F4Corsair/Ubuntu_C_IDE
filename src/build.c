#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <curses.h>
#include <fcntl.h>
#include <sys/wait.h>

#include "build.h"
#include "global.h"
#include "uibase.h"

#define PIPE_ENDS 2

WINDOW* opened_build_tab;

// 한 번에 한 개의 파일만 complie, debug 할 수 있다고 가정 하고 제작함.
void build_tab_transition() {
    if(menu_tab_focus == BUILD_TAB)
        return;

    erase();
    refresh();

    menu_tab_focus = BUILD_TAB;
    menu_tab_update();
    wrefresh(menu_tab);

    // todo : show BUILD_TAB
    // opened_build_tab_print();
    build_compile_print();
}

// todo : compile 결과 표시 하기
void build_compile_print() {
    int the_pipe[PIPE_ENDS], read_len;
    char buf[BUFSIZ]; char executable_file_name[256];
    FileStatus *status = opened_file_info->focus;
    wclear(contents);

    // executable file name to implement makefile 
    strcpy(executable_file_name, status->file_name);
    char *dot = strrchr(executable_file_name, '.');
    *(dot + 1) = '\0';
    strcat(executable_file_name, ".");
    strcat(executable_file_name, "out");

    if (status == NULL) {
        // no file opened
        mvwaddstr(contents, (win_row - 3) / 2 - 1, win_col / 2 - 18, "Open File from File Tab [Ctrl + F]");
        mvwaddstr(contents, (win_row - 3) / 2, win_col / 2 - 17, "If you are first, try [Ctrl + M]");
    } 
    else if(status->fd == -1) {
        mvwaddstr(contents, (win_row - 3) / 2, win_col / 2 - 10, "File does not exist");
    } 
    else {
        // print to contents - using pipe!
        if (pipe(the_pipe) == -1) {
            perror("pipe");
            exit(1);
        }

        switch (fork()) {
            case -1:
                perror("fork");
                exit(1);
            case 0: // child = only writing makefile's results
                close(the_pipe[0]);
                dup2(the_pipe[1], 1);
                dup2(the_pipe[1], 2);
                close(the_pipe[1]);
                // todo : makefile create + makefile 지정
                execlp("make", "make", NULL);
                perror("make");
                exit(5);
                break;
            default: // parent = write results from child's make command on window
                close(the_pipe[1]);
                dup2(the_pipe[0], 0);
                close(the_pipe[0]);
                int row = 0;
                while ((read_len = read(the_pipe[0], buf, BUFSIZ)) > 0) {
                    mvwprintw(contents, row++, 0, "%s", buf);
                }
        }
    }

    wrefresh(contents);
}

/* void build_debug_print() {
    int the_pipe[PIPE_ENDS], newfd, pid, read_len;
    char buf[BUFSIZ];
    FileStatus *status = opened_file_info->focus;
    wclear(contents);

    if (status == NULL) {
        // no file opened
        mvwaddstr(contents, (win_row - 3) / 2 - 1, win_col / 2 - 18, "Open File from File Tab [Ctrl + F]");
        mvwaddstr(contents, (win_row - 3) / 2, win_col / 2 - 17, "If you are first, try [Ctrl + M]");
    } 
    else if(status->fd == -1) {
        mvwaddstr(contents, (win_row - 3) / 2, win_col / 2 - 10, "File does not exist");
    } 
    else {
        // print to contents - using pipe!
    }

    wrefresh(contents);
} */

void opened_build_tab_print() {
    
}
