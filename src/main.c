#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <panel.h>

#include "global.h"
#include "uibase.h"
#include "winsize.h" // handles SIGWINCH
#include "input.h"

#define DEBUG_INPUT // turn on debug mode

int main(int argc, char **argv) {
	int input_char;

	// debug variable declare


	// stream.h -> connect stdout, stderr to file : log it
	ui_init();
	winsize_init(); // handle SIGWINCH
	ui_set_whole();

	while(1) {
		// read key input
		input_char = getch();
		if(input_control(input_char) == -1)
			break;
		// break; // for debug
	}

	// terminate procedure
	ui_terminate();

	return 0;
}
