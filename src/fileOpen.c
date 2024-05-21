#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "global.h"
#include "code.h"
//#include "layout.h"
#include "winsize.h"

void file_open_update() {
	winsize_calculate();
	wclear(file_contents);

	int row_pos = win_row / 2 - 2;
    mvwaddstr(file_contents, row_pos++, win_col / 2 - 10, "Code file tab is full!");
	mvwaddstr(file_contents, row_pos++, win_col / 2 - 15, "Do you really want to open the file?");
    int col_pos = win_col / 2 - 3;
    mvwaddch(file_contents, row_pos, col_pos++, '[');
    wattron(file_contents, A_UNDERLINE);
    mvwaddch(file_contents, row_pos, col_pos++, 'Y');
    wattroff(file_contents, A_UNDERLINE);
    mvwaddstr(file_contents, row_pos, col_pos, "es]");
}

void file_open(char *file_name) {
	FileStatus temp;
	int new_file_input;
	
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
        	temp.file_name = strdup(file_name);
        	temp.full_path = strdup(path);	
	}
	else {
		perror("file does not exist");
		exit(1);
	}

	// to do : file name 넘겨주기 + file tab number handling
	if (new_opened_file_tab(temp.file_name, temp.full_path) == -1) {
		file_open_update();
		
		wscanw(file_contents, "%d", &input);
		if (input == 'y' || input = 'Y')
			del_opened_file_tab(0); // delete first-opened code file tab
		else {
			// file contents window 원상 복구
			getcwd(path, 256);
			//displayDirectoryContents(file_contents, path); // 영준이 demo 함수 수정 필요
		}
	}
}	
