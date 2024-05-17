#include "global.h"

// SIGWINCH
int winsize_flag; // 0 : default  1 : blocked  2 : ready to refresh

// menu tab
enum MenuTab menu_tab_focus; // will define its value at menuTab.h
enum MenuTab menu_tab_focus_backup[2]; // 0 : for quit tab  1 : for winsize changed

// size information of stdwin
int win_row;
int win_col;