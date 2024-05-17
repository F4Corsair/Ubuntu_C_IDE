#include <stdio.h>
#include <unistd.h>

#include "input.h"
#include "global.h"
#include "uibase.h"
#include "code.h"
#include "file.h"
#include "build.h"
#include "terminal.h"
#include "manual.h"
#include "quit.h"
#include "winsize.h"


int input_control(int input_char) {
#ifdef DEBUG_INPUT
		// print input keyname
		// wattron(file_tab, A_UNDERLINE);
		// mvwprintw(file_tab, 0, 0, "%-*s", KEY_NAME_WIDTH - 1, keyname(input_char));
		// wattroff(file_tab, A_UNDERLINE);
		// wrefresh(file_tab);
#endif
    // input ignore
    if (input_char == 0x19a || winsize_flag == 1) {
        // ignore KEY_RESIZE
        return 0;
    }
    // check tab transition
    if (input_char == CTRL('q')) { // terminate program
        quit_tab_transition();
        return 0;
    }
    switch (input_char)
    {
    case CTRL('c'):
        code_tab_transition();
        return 0;
    case CTRL('f'):
        file_tab_transition();
        return 0;
    case CTRL('b'):
        build_tab_transition();
        return 0;
    case CTRL('t'):
        terminal_tab_transition();
        return 0;
    case 0xa: // to recognize CTRL_M
        manual_tab_transition();
        return 0;
    case CTRL('q'):
        quit_tab_transition();
        return 0;
    default:
        break;
    }

    if(menu_tab_focus == CODE_TAB) { // code tab requires numerous input - so check it first
        // print input char
    } else {
        switch (menu_tab_focus)
        {
        case FILE_TAB:
            break;
        case BUILD_TAB:
            break;
        case TERMINAL_TAB:
            break;
        case MANUAL_TAB:
            break;
        case QUIT_TAB:
            if(input_char == 'y' || input_char == 'Y') {
                return -1;
            } else {
                tab_restore();
            }
            break;
        case WINSIZE_TAB:
            if(winsize_flag == 2) {
                // ready to reset stdscr
                winsize_calculate();
                window_reset();
                tab_restore();
            }
            break;
        default:
            perror("menu_tab_focus out of index");
        }

    }

    return 0;
}