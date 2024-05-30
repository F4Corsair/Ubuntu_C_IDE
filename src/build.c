#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <curses.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "build.h"
#include "file.h"
#include "global.h"
#include "uibase.h"

#define PIPE_ENDS 2

typedef struct compile results;

struct compile {
	char line[BUFSIZ];
	results* next;
};

// 한 번에 한 개의 파일만 complie, debug 할 수 있다고 가정 하고 제작함.
void build_tab_transition() {
    if(menu_tab_focus == BUILD_TAB)
        return;

    erase();
    refresh();

    menu_tab_focus = BUILD_TAB;
    menu_tab_update();
    wrefresh(menu_tab);

    opened_workspace_tab_print();
    build_compile_print();
}

void append(results **head_ref, char *data) {
    results *new_node = (results*)malloc(sizeof(results));
    if (new_node == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }
    strcpy(new_node->line, data);
    new_node->next = NULL;

    if (*head_ref == NULL) {
        *head_ref = new_node;
    } else {
        results *last_node = *head_ref;
        while (last_node->next != NULL) {
            last_node = last_node->next;
        }
        last_node->next = new_node;
    }
}

// todo : compile 결과 표시 하기
void build_compile_print() {
    int read_len;
    char buf[BUFSIZ]; char executable_file_name[256];
    FILE *pipe_fp;

    if (opened_file_info == NULL) {
        fprintf(stderr, "Error: opened_file_info is NULL\n");
        return;
    }
    
    FileStatus *status = opened_file_info->focus;
    wclear(contents);

    // executable file name to implement makefile 
    if (status) {
	strcpy(executable_file_name, status->file_name);
    	char *dot = strrchr(executable_file_name, '.');
    	*(dot + 1) = '\0';
    	strcat(executable_file_name, ".");
    	strcat(executable_file_name, "out");
    }

    if (status == NULL) {
        // no file opened
        mvwaddstr(contents, (win_row - 3) / 2 - 1, win_col / 2 - 18, "Open File from File Tab [Ctrl + F]");
        mvwaddstr(contents, (win_row - 3) / 2, win_col / 2 - 17, "If you are first, try [Ctrl + M]");
    } 
    else if(status->fd == -1) {
        mvwaddstr(contents, (win_row - 3) / 2, win_col / 2 - 10, "File does not exist");
    } 
    else {
	results *compile_result = (results*)malloc(sizeof(results));

	pipe_fp = popen("make", "r"); // "make" 명령어를 실행하고 해당 출력을 읽을 파일 스트림을 생성
        if (pipe_fp == NULL) {
            perror("popen");
            exit(1);
        }

	int row = 1;
	mvwprintw(contents, row++, 0, "compile results:");
        while (fgets(buf, BUFSIZ, pipe_fp) != NULL) { // 프로세스의 출력을 한 줄씩 읽어옴
		// already up to date - handling

		// append(&compile_result, buf);
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
	
    }

    wrefresh(contents);
}
