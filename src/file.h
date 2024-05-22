#ifndef __FILE_TAB
#define __FILE_TAB

#include "global.h"

void file_tab_transition();
void file_open_update();
int find_most_previous_file();
void contents_window_restore();
void file_open(char *file_name, int new_file_input);
void opened_workspace_tab_print();
void workspace_contents_print();

#endif