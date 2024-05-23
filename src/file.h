#ifndef __FILE_TAB
#define __FILE_TAB

#include "global.h"
#include "code.h"
#include "winsize.h"
#include "openedFileTab.h"
#include "file.h"
#include "uibase.h"

#define KEY_NAME_WIDTH 14
#define FILE_TAB_WIDTH 15

void file_tab_transition();
void file_open_update();
int find_most_previous_file();
void contents_window_restore();
void file_open(char *file_name, int new_file_input);
void opened_workspace_tab_print();
void workspace_contents_print();
void print_path(const char *path);
void ls( char *path);
void addToList(FileStatus **head,char *file_name, char *full_path);
void lsR(char *path);
void initialize_colors();
void workspace_key_down();
void workspace_key_up();
bool has_extension(const char *filename, const char *extension);
void new_file_tab();

#endif
