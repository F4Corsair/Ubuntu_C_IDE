#ifndef __OPENED_FILE_TAB
#define __OPENED_FILE_TAB

#include "global.h"

extern int unsaved_caution_flag;

void opened_file_tab_print();
int new_opened_file_tab(char *file_name, char *full_path);
void del_opend_file_tab_fileStatus(FileStatus *ptr);
void del_opened_file_tab(int idx);
OpenFileInfo *opened_file_info_init();
void opened_file_info_terminate();
int opened_file_focus_idx_find();
void opened_file_focus_next();
void opened_file_focus_prev();
void close_unsaved_caution();
off_t get_file_size(int fd) ;

#endif