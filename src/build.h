#ifndef __BUILD_TAB
#define __BUILD_TAB

#include "global.h"
#include "uibase.h"

#define PIPE_ENDS 2

void build_tab_transition();
void build_compile_print();
// void build_debug_print();

extern int max_file_tab;
extern int file_tab_cnt;
extern int file_tab_focus;
extern WorkSpaceFile* filetab_head;

#endif
