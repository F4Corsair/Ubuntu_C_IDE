// all golbal variables & struct usually shared or play critical role between codes shouled defined at here
#ifndef __GlobalHeader
#define __GlobalHeader

/* MACRO FUNCTIONS */
#define CTRL(c) ((c) & 037)  // macro function to read CTRL + character

/* STRUCT */

typedef struct {
	char *file_name;
	char *full_path;
    int row, col;
} FileStatus; // saves opened file status - uses at file open & menu tab print ...

/* GLOBAL VARIABLES */

// file tab
int max_file_tab; // cur window size determine this

// menu tab
int menu_tab_focus; // will define its value at menuTab.h

// size information of stdwin
int win_row;
int win_col;

#endif