#include <curses.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "global.h"
#include "code.h"
#include "uibase.h"

void code_tab_transition() {
    if(menu_tab_focus == CODE_TAB)
        return;

    erase();
    refresh();

    unsaved_caution_flag = 0; // forget about unsaved handle procedure
    menu_tab_focus = CODE_TAB;
    menu_tab_update();
    wrefresh(menu_tab);

    opened_file_tab_print();
    code_contents_print();
}

void code_contents_print() {
    FileStatus *status = opened_file_info->focus;
    
    wclear(contents);

    if (status == NULL) {
        // no file opened
        mvwaddstr(contents, (win_row - 3) / 2 - 1, win_col / 2 - 18, "Open File from File Tab [Ctrl + F]");
        mvwaddstr(contents, (win_row - 3) / 2, win_col / 2 - 17, "If you are first, try [Ctrl + M]");
    } else if(status->fd == -1) {
        mvwaddstr(contents, (win_row - 3) / 2, win_col / 2 - 10, "File does not exist");
    } else {
        // buffer code if not exist - check row & col
        // print to contents from code
        mvwprintw(contents, 0, 0, "%d", status->buf_cnt);
    }

    wrefresh(contents);
}

void code_buf_initialize(FileStatus *status) {
    char read_buf[BUFSIZ + 1];
    read_buf[BUFSIZ] = '\0';
    status->buf_prev = NULL;

    int read_len = read(status->fd, read_buf, BUFSIZ);
    if(read_len == -1) {
        perror("code_buf_initialize : failed to read");
        return;
    }
    read_buf[read_len] = '\0'; // just in case (prevent error)
    status->buf_cur = parse_buf(read_buf, read_len);

    if(status->buf_cnt > 1) {
        read_len = read(status->fd, read_buf, BUFSIZ);
        if(read_len == -1) {
            perror("code_buf_initialize : failed to read");
            return;
        }
        read_buf[read_len] = '\0'; // just in case (prevent error)
        status->buf_next = parse_buf(read_buf, read_len);
        if(status->buf_cur->end_with_new_line == 1) {
            status->buf_next->head_row += status->buf_cur->tail_row + 1;
            status->buf_next->tail_row += status->buf_cur->tail_row + 1;
        }
        else {
            status->buf_next->head_row += status->buf_cur->tail_row;
            status->buf_next->tail_row += status->buf_cur->tail_row;
        }
        status->buf_next->cur_row = status->buf_next->head_row;
    }
}

CodeBuf *parse_buf(char *buf, int read_len) {
    char *start, *end;
    int row = 0;


    CodeBuf *code_buf = malloc(sizeof(CodeBuf));
    code_buf->cur_row = code_buf->head_row = 0;

    if(buf == NULL) {
        code_buf->tail_row = 0;
        code_buf->end_with_new_line = 0;
        code_buf->head = code_buf->tail = NULL;
        return code_buf;
    }
    // make head
    start = strchr(buf, '\n');
    code_buf->head = code_line_append(buf, start);
    code_buf->head->prev = NULL;
    row++;
    CodeLine *pre = code_buf->head;
    CodeLine *cur = NULL;
    end = strchr(start + 1, '\n');
    while(end != NULL) {
        cur = code_line_append(start + 1, end);
        pre->next = cur;
        row++;
        pre = cur;
        start = end;
        end = strchr(start + 1, '\n');
    }
    // make tail
    end = strchr(start, '\0');
    cur = code_line_append(start + 1, end);
    pre->next = cur;
    row++;

    if(cur == NULL) {
        code_buf->tail = code_buf->head;
    }
    code_buf->tail = cur;
    code_buf->tail->next = NULL;

    code_buf->tail_row = row;
    
    if(buf[read_len] == '\n') {
        code_buf->end_with_new_line = 1;
    } else {
        code_buf->end_with_new_line = 0;
    }

    return code_buf;
}

CodeLine* code_line_append(char *start, char *end) {
    int len = end - start;
    CodeLine *code_line = malloc(sizeof(CodeLine));

    code_line->line = malloc(sizeof(char) * (len + 1));
    strncpy(code_line->line, start, len);
    code_line->line[len] = '\0';

    // for debug
    fprintf(stderr, "line : %s\n", code_line->line);

    return code_line;
}


void file_status_close(FileStatus *ptr) {
    if(ptr->fd != -1) {
        close(ptr->fd);
        code_buf_close(ptr->buf_prev);
        code_buf_close(ptr->buf_cur);
        code_buf_close(ptr->buf_next);
    }
    free(ptr);
}

void code_buf_close(CodeBuf *buf) {
    if(buf == NULL)
        return;

    CodeLine *ptr = buf->head;
    CodeLine *next = NULL;
    while(ptr != NULL) {
        next = ptr->next;
        free(ptr->line);
        free(ptr);
        ptr = next;
    }
    free(buf);
}