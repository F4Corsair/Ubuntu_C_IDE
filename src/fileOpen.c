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
//#include "layout.h"
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

void file_open(char *file_name, int new_file_input) {
	FileStatus temp;
	// int new_file_input;
	char path[256];
	
	if (chdir(".") != 0) {
		perror("directory");
		exit(1);
	}

	// full_path
	if (getcwd(path, 256) == NULL) {
		perror("getcwd");
		exit(1);
	}
	strcat(path, "/");
	strcat(path, file_name);
	
	// file existence check - exception handling
	if (access(path, F_OK) == 0) {
		// initializing
        	strcpy(temp.file_name, strdup(file_name));
        	strcpy(temp.full_path, strdup(path));	
	}
	else {
		perror("file does not exist");
		exit(1);
	}

	// to do : file name 넘겨주기 + file tab number handling
	if (new_opened_file_tab(temp.file_name, temp.full_path) == -1) {
		file_open_update();
		
		// new_file_input = getch();
		if (new_file_input == 'y' || new_file_input == 'Y') {
			del_opened_file_tab(8);	// delete first-opened code file tab
			new_opened_file_tab(temp.file_name, temp.full_path);
			opened_file_focus_prev();
			opened_file_tab_print();
			code_contents_print();
		}
		else {
			tab_restore();
		}
	}
}	
