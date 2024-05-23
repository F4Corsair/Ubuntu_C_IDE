#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>

#include "global.h"
#include "uibase.h"
#include "winsize.h" // handles SIGWINCH
#include "input.h"
#include "errLog.h"
#include "code.h" // for TEST

int main(int argc, char **argv) {
	int input_char;

	start_logging();
	ui_init();
	winsize_calculate();
	ui_set_whole();
	winsize_init(); // handle SIGWINCH

	// debug variable declare
	new_opened_file_tab("input.h", "input.h");
	new_opened_file_tab("input.c", "input.c");
	new_opened_file_tab("testFileNameIsLong", "");
	new_opened_file_tab("testFileNameIsLong2", "");
	new_opened_file_tab("testFile.c", "");
	new_opened_file_tab("testFile2.c", "");
	new_opened_file_tab("code_test_file.log", "code_test_file.log");

	while(1) {
		input_char = getch();
		if(input_control(input_char) == -1)
			break;
	}

	// terminate procedure
	ui_terminate();
	end_logging();

	return 0;
}
