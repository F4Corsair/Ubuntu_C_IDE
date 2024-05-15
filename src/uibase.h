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
extern int file_tab_cnt; // number of current file tab
extern int file_tab_focus; // don't change it directly

extern WINDOW *file_tab;
extern WINDOW *menu_tab;
extern WINDOW *contents;

/* METHOD */
void ui_init();
void ui_terminate();
void ui_set_whole();
void menu_tab_reset();
void menu_tab_update();
void tab_restore();

#endif

// notice
/*
 * don't change menu_tab_focus directly
*/