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
        int cur_row = status->row - status->start_row;
        int cur_col = status->col - status->start_col;
        int row;
        CodeLine *ptr;

        // print to contents
        ptr = status->buf->cur;
        for(row = 0; row < win_row - 3; row++) {
            mvwprintw(contents, row, 0, "%s", &(ptr->line[status->col]));
            if(ptr->next != NULL) {
                ptr = ptr->next;
            } else {
                break;
            }
        }
        wrefresh(contents);

        // cursor highlight
        wattron(contents, A_BLINK | A_STANDOUT);
        ptr = status->buf->cur;
        for(row = 0; row < win_row; row++) {
            if(row == cur_row) {
                char c = ptr->line[status->col];
                if(c == '\0') {
                    mvwprintw(contents, cur_row, cur_col, " ");
                } else {
                    mvwprintw(contents, cur_row, cur_col, "%c", c);
                }
            }
            if(ptr->next != NULL) {
                ptr = ptr->next;
            } else {
                break;
            }
        }
        wattroff(contents, A_BLINK | A_STANDOUT);
    }
    wrefresh(contents);

}

void code_buf_initialize(FileStatus *status) {
    char read_buf[BUFSIZ + 1];
    read_buf[BUFSIZ] = '\0';

    int read_len = read(status->fd, read_buf, BUFSIZ);
    if(read_len == -1) {
        perror("code_buf_initialize : failed to read");
        return;
    }
    read_buf[read_len] = '\0'; // just in case (prevent error)
    
    status->buf = malloc(sizeof(CodeBuf));
    status->buf->cur_row = status->buf->tail_row = 0;
    status->buf->head = status->buf->tail = status->buf->cur = NULL;
    status->buf->end_with_new_line = 1;
    parse_buf(status->buf, read_buf, read_len);

    for(int i = 1; i < status->buf_cnt; i++) {
        read_len = read(status->fd, read_buf, BUFSIZ);
        if(read_len == -1) {
            perror("code_buf_initialize : failed to read");
            return;
        }
        read_buf[read_len] = '\0'; // just in case (prevent error)
        parse_buf(status->buf, read_buf, read_len);
    }
}

void parse_buf(CodeBuf *code_buf ,char *buf, int read_len) {
    char *start, *end;
    int row = 0;

    if(buf == NULL) {
        return;
    }
    // make head
    CodeLine *pre = code_buf->cur;
    CodeLine *cur = NULL;
    start = strchr(buf, '\n');
    if(start == NULL) { // file has only a line
        start = strchr(buf, '\0');
        cur = code_line_append(buf, start);
        if(pre == NULL) {
            code_buf->tail_row++;
            code_buf->head = code_buf->tail = code_buf->cur = cur;
            code_buf->head->prev = code_buf->head->next = NULL;
            return;
        }
        if(code_buf->end_with_new_line == 0) {
            code_buf->cur->line = realloc(code_buf->cur->line, strlen(code_buf->cur->line) + strlen(cur->line) + 1);
            strcat(code_buf->cur->line, cur->line);
            return;
        }
        pre->next = cur;
        cur->prev = pre;
        code_buf->cur = cur;
        code_buf->tail = cur;
        code_buf->tail_row++;
        return;
    } 

    if(pre == NULL) {
        code_buf->head = code_line_append(buf, start);
        code_buf->head->prev = NULL;
        code_buf->cur = code_buf->head;
        pre = code_buf->cur;
    } else {
        if(code_buf->end_with_new_line == 0) {
            code_buf->cur->line = realloc(code_buf->cur->line, strlen(code_buf->cur->line) + strlen(cur->line) + 1);
            strcat(code_buf->cur->line, cur->line);
            pre = code_buf->cur;
            row--;
        } else {
            cur = code_line_append(buf, start);
            pre->next = cur;
            cur->prev = pre;
            pre = cur;
        }
    }
    row++;
    end = strchr(start + 1, '\n');
    while(end != NULL) {
        cur = code_line_append(start + 1, end);
        pre->next = cur;
        cur->prev = pre;
        row++;
        pre = cur;
        start = end;
        end = strchr(start + 1, '\n');
    }
    // make tail
    end = strchr(start, '\0');
    cur = code_line_append(start + 1, end);
    pre->next = cur;
    cur->prev = pre;
    row++;

    // if(cur == NULL) {
    //     code_buf->tail = code_buf->head;
    // }
    code_buf->tail = cur;
    code_buf->tail->next = NULL;

    code_buf->tail_row += row;
    
    if(buf[read_len] == '\n') {
        code_buf->end_with_new_line = 1;
    } else {
        code_buf->end_with_new_line = 0;
    }
}

CodeLine* code_line_append(char *start, char *end) {
    int len = end - start;
    CodeLine *code_line = malloc(sizeof(CodeLine));

    code_line->line = malloc(sizeof(char) * (len + 1));
    strncpy(code_line->line, start, len);
    code_line->line[len] = '\0';
    code_line->len = len;

    // for debug
    // fprintf(stderr, "line : %s\n", code_line->line);

    return code_line;
}


void file_status_close(FileStatus *ptr) {
    if(ptr->fd != -1) {
        close(ptr->fd);
        code_buf_close(ptr->buf);
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

int code_next_row_exists() {
    FileStatus *focus = opened_file_info->focus;
    int row = focus->row;
    if (row + 1 >= focus->buf->tail_row)
        return -1;
    else
        return 0;
}