// all golbal variables & struct usually shared or play critical role between codes shouled defined at here
#ifndef __GlobalHeader
#define __GlobalHeader

/* MACRO CONSTANT */
// flag offset
#define WINSIZE_CHANGED 0x0
#define WAIT_ANY_INPUT 0x1

/* MACRO FUNCTIONS */
#define CTRL(c) ((c) & 037)  // macro function to read CTRL + character

/* STRUCT */

typedef struct {
	char *file_name;
	char *full_path;
    int row, col;
} FileStatus; // saves opened file status - uses at file open & menu tab print ...

enum MenuTab {
    CODE_TAB,
    FILE_TAB,
    BUILD_TAB,
    TERMINAL_TAB,
    MANUAL_TAB,
    QUIT_TAB
};

/* GLOBAL VARIABLES */

// status flag - access with bitwise operation(to optimize)
int flag;

// file tab
int max_file_tab; // cur window size determine this

// menu tab
enum MenuTab menu_tab_focus; // will define its value at menuTab.h

// size information of stdwin
int win_row;
int win_col;

/* METHOD */

// flag
void flag_init();
int get_winsize_changed();
void set_winsize_changed(int val);
int get_wait_any_input();
void set_wait_any_input(int val);

#endif