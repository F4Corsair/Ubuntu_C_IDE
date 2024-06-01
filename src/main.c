#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>

#include "global.h"
#include "uibase.h"
#include "winsize.h" // handles SIGWINCH
#include "input.h"
#include "errLog.h"

int main(int argc, char **argv) {
	int input_char;

	start_logging();
	ui_init();
	winsize_calculate();
	ui_set_whole();
	winsize_init(); // handle SIGWINCH

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
