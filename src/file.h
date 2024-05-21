#ifndef __FILE_TAB
#define __FILE_TAB

#define KEY_NAME_WIDTH 14
#define FILE_TAB_WIDTH 15

#include "global.h"

void file_tab_transition();
void newFileTab(WINDOW *file_tab);
void printPath(WINDOW *path_win, char *path);

WINDOW *file_contents;
WINDOW *file_path;
WINDOW *file_tab;
//WINDOW *file_menu_tab;

int max_file_tab; // cur window size determine this
int file_tab_cnt;
int file_tab_focus;

#endif