#include <curses.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>

#include "global.h"
#include "code.h"
#include "winsize.h"
#include "openedFileTab.h"
#include "file.h"
#include "uibase.h"

void file_tab_transition() {
    if(menu_tab_focus == FILE_TAB)
        return;

    erase();
    refresh();

    menu_tab_focus = FILE_TAB;
    menu_tab_update();
    wrefresh(menu_tab);

    // todo : show FILE_TAB
    opened_workspace_tab_print();
    workspace_contents_print();
}

void file_open_update() {
	winsize_calculate();
	wclear(contents);

	int row_pos = win_row / 2 - 2;
   	mvwaddstr(contents, row_pos++, win_col / 2 - 10, "Code file tab is full!");
	mvwaddstr(contents, row_pos++, win_col / 2 - 15, "Do you really want to open the file?");
    int col_pos = win_col / 2 - 3;
    	mvwaddch(contents, row_pos, col_pos++, '[');
    	wattron(contents, A_UNDERLINE);
    	mvwaddch(contents, row_pos, col_pos++, 'Y');
    	wattroff(contents, A_UNDERLINE);
    	mvwaddstr(contents, row_pos, col_pos, "es]");
}

int find_most_previous_file() {
	FileStatus *cur = opened_file_info->head;
    FileStatus *most_previous_file = NULL;

	// head가 가리키는 node의 prev node가 가장 나중에 만들어진 node
	int index = 0;
	do {
		most_previous_file = cur;
		cur = cur->next;
		index++;
	} while (cur->next != opened_file_info->head);

	return index;
}

void contents_window_restore() {
	enum MenuTab focus;
	wclear(contents);

	if (menu_tab_focus == CODE_TAB) {
		opened_file_tab_print();
		code_contents_print();
	}
	else if (menu_tab_focus == FILE_TAB) {
		// todo : add function : file.c에서 만든 사용자 정의 함수
		opened_workspace_tab_print();
		workspace_contents_print();
	}
}

void file_open(char *file_name, int new_file_input) {
	// int new_file_input;
	char path[256];

	// full_path
	if (getcwd(path, 256) == NULL) {
		perror("getcwd");
		exit(1);
	}
	strcat(path, "/");
	strcat(path, file_name);
	
	// file existence check - exception handling
	if (access(path, F_OK) != 0) {
		perror("file does not exist");
		exit(1);
	}

	// to do : input control
	if (new_opened_file_tab(file_name, path) == -1) {
		file_open_update();
		
		// new_file_input = getch();
		if (new_file_input == 'y' || new_file_input == 'Y') {
			// todo : index 지정 정확하게 하기
			del_opened_file_tab(find_most_previous_file());
			new_opened_file_tab(file_name, path);
			opened_file_tab_print();
			code_contents_print();
		}
		else // todo : make new contents restore code
			contents_window_restore();
	}
}	

// 영준이 todo
void opened_workspace_tab_print() {

}

void workspace_contents_print() {

}
