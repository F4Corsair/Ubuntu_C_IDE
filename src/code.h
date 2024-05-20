#ifndef __CODE_TAB
#define __CODE_TAB

#include "global.h"
#include "openedFileTab.h"

void code_tab_transition();
void code_contents_print();
void code_buf_initialize(FileStatus *status);
CodeBuf *parse_buf(char *buf, int read_len);
CodeLine* code_line_append(char *start, char *end);
void file_status_close(FileStatus *ptr);
void code_buf_close(CodeBuf *buf);
int code_next_row_exists();

#endif