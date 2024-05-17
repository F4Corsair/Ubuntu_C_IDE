#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>

#include "global.h"
#include "uibase.h"
#include "winsize.h" // handles SIGWINCH
#include "input.h"
#include "errLog.h"

#define DEBUG_INPUT // turn on debug mode

int main(int argc, char **argv) {
	int input_char;

	// debug variable declare


	start_logging();
	ui_init();
	winsize_init(); // handle SIGWINCH
	ui_set_whole();

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
