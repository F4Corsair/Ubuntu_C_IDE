#ifndef __UI_BASE
#define __UI_BASE

#include "global.h"

/* MACRO CONSTANT */
#define DEBUG_INPUT // debug option : print input charcter

// lower bound of menu & file tab width
#define KEY_NAME_WIDTH 14
#define FILE_TAB_WIDTH 15
#define MIN_MENU_TAB_WIDTH 10
#define MENU_TAB_CNT 6

// lower bound of stdscr
#define WIN_MIN_WIDTH (MIN_MENU_TAB_WIDTH * MENU_TAB_CNT)
#define WIN_MIN_HEIGHT 30
// todo : re-calculate this value after UI design is done

/* STRUCT */

/* READ ONLY DATA */
static char menu_tab_names[MENU_TAB_CNT][MIN_MENU_TAB_WIDTH - 1] = {"  Code  ", "  File  ", "  Build ", "Terminal", " Manual ", "  Quit  "};
static int menu_tab_pos[MENU_TAB_CNT] = {2, 2, 2, 0, 1, 2};  // start position offset corresponding to each menu tab

/* GLOBAL VARIABLE */
extern int max_file_tab; // window size determine this
int file_tab_cnt; // # of current file tab
int file_tab_focus;

/* METHOD */
void ui_init();
void ui_terminate();

#endif