#ifndef __WINSIZE_CHANGE_HANDLE
#define __WINSIZE_CHANGE_HANDLE

#include "global.h"

// todo : re-calculate this value after UI design is done
#define WIN_MIN_ROW 30
#define WIN_MIN_COL 80

extern int win_row;
extern int win_col;

void winsize_init();
void wininch_handler(int signum);
void winsize_calculate();

#endif