#include <stdio.h>
#include <unistd.h>

#include "input.h"
#include "global.h"

// todo : re-design it!

int input_control(int input_char) {
#ifdef DEBUG_INPUT
		// print input keyname
		// wattron(file_tab, A_UNDERLINE);
		// mvwprintw(file_tab, 0, 0, "%-*s", KEY_NAME_WIDTH - 1, keyname(input_char));
		// wattroff(file_tab, A_UNDERLINE);
		// wrefresh(file_tab);
#endif
    if (input_char == CTRL('q'))
        return -1;

    // 1. flag check  2. menu tab check  3. input check
    if(flag == 0) {
        switch (menu_tab_focus)
        {
        case CODE_TAB:
            break;
        case FILE_TAB:
            break;
        case BUILD_TAB:
            break;
        case TERMINAL_TAB:
            break;
        case MANUAL_TAB:
            break;
        case QUIT_TAB:
            break;
        default:
            perror("menu_tab_focus out of index");
        }

    } else {
        if(get_winsize_changed()) {

        }
        if(get_wait_any_input()) {
            // have to change default mode & refresh window
        }
    }
    return 0;
}