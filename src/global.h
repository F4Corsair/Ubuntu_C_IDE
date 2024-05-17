// all golbal variables & struct usually shared or play critical role between codes shouled defined at here
#ifndef __GlobalHeader
#define __GlobalHeader

#include <curses.h>
#include <time.h>

/* MACRO CONSTANT */
// flag offset
#define WINSIZE_CHANGED 0x0
#define WAIT_ANY_INPUT 0x1

/* STRUCT */

typedef struct {
	char file_name[256];
	char full_path[256];
    int row, col;
    int modified; // 0 : unmodified  1 : tmp saved  2 : modified
    time_t last_saved;
} FileStatus; // saves opened file status - uses at file open & menu tab print ...

enum MenuTab {
    CODE_TAB,
    FILE_TAB,
    BUILD_TAB,
    TERMINAL_TAB,
    MANUAL_TAB,
    QUIT_TAB,
    WINSIZE_TAB
};

/* GLOBAL VARIABLES */

// global.c
extern int winsize_flag; // 0 : default  1 : blocked  2 : ready to refresh

extern enum MenuTab menu_tab_focus; // will define its value at menuTab.h
extern enum MenuTab menu_tab_focus_backup[2];

extern int win_row;
extern int win_col;

// uibase.h
extern int max_file_tab;
extern int opened_file_tab_cnt;
extern int opened_file_tab_focus;

extern WINDOW *opened_file_tab;
extern WINDOW *menu_tab;
extern WINDOW *contents;

#endif

/* MACRO FUNCTIONS */
#ifndef CTRL
#define CTRL(c) ((c) & 037)// macro function to read CTRL + character
#endif