#ifndef __CODE_TAB
#define __CODE_TAB

#include "global.h"

void code_tab_transition();

void opened_file_tab_print();
int new_opened_file_tab(char *file_name, char *full_path);
void del_opened_file_tab(int idx);
OpenFileInfo *opened_file_info_init();
void opened_file_info_terminate();
void opened_file_focus_next();
void opened_file_focus_prev();
int close_unsaved_caution();

#endif