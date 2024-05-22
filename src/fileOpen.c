#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "global.h"
#include "code.h"
#include "openedFileTab.h"
#include "uibase.h"
#include "winsize.h"

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
			opened_file_focus_prev();
			opened_file_tab_print();
			code_contents_print();
		}
		else {
			// todo : make new contents restore code
			tab_restore();
		}
	}
}	
