#ifndef __WINSIZE_CHANGE_HANDLE
#define __WINSIZE_CHANGE_HANDLE

#include "global.h"

// window lower bound : WIN_MIN_HEIGHT & WIN_MIN_WIDTH (defined on global.h)

extern int win_row;
extern int win_col;

void winsize_init();
void wininch_handler(int signum);
void winsize_calculate();

#endif