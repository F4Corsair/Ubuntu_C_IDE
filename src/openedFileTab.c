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

int unsaved_caution_flag;

void opened_file_tab_print() {
    if(opened_file_info->cnt > MAX_FILE_TAB_CNT) {
        perror("opend_file_tab out of index");
        return;
    }

    wclear(opened_file_tab);
    wrefresh(opened_file_tab);

    wattron(opened_file_tab, A_UNDERLINE);
    mvwaddstr(opened_file_tab, 0, 0, "ROW#|COL# ");
    wattroff(opened_file_tab, A_UNDERLINE);
    mvwaddch(opened_file_tab, 0, INFO_LABEL_WIDTH - 1, '/');

    int col = INFO_LABEL_WIDTH;
    int focus_len = FILE_TAB_FOCUS_MAX_WIDTH - 2;
    int len;
    if(opened_file_info->cnt != 0)
        len = (win_col - INFO_LABEL_WIDTH - focus_len) / opened_file_info->cnt;
    len = len > focus_len ? focus_len : len;
    FileStatus *ptr = opened_file_info->head;
    for(int i = 0; i < opened_file_info->cnt; i++) {
        mvwaddch(opened_file_tab, 0, col++, '\\');
        wattron(opened_file_tab, A_UNDERLINE);
        if(ptr == opened_file_info->focus) {
            wattron(opened_file_tab, A_STANDOUT);
            if(ptr->modified != 0)
                mvwaddch(opened_file_tab, 0, col++, '*');
            else
                mvwaddch(opened_file_tab, 0, col++, ' ');
            mvwprintw(opened_file_tab, 0, col, "%-*s", focus_len - 1, ptr->file_name);
            col = col + focus_len - 1;
            wattroff(opened_file_tab, A_STANDOUT);
        } else {
            if(ptr->modified != 0)
                mvwaddch(opened_file_tab, 0, col++, '*');
            else
                mvwaddch(opened_file_tab, 0, col++, ' ');
            mvwprintw(opened_file_tab, 0, col, "%-*s", len - 1, ptr->file_name);
            col = col + len - 1;
        }
        wattroff(opened_file_tab, A_UNDERLINE);
        mvwaddch(opened_file_tab, 0, col++, '/');
        ptr = ptr->next;
    }
    for(int i = col; i < win_col; i++) {
        mvwaddch(opened_file_tab, 0, i, ' ');
    }
    wrefresh(opened_file_tab);
}

int new_opened_file_tab(char *file_name, char *full_path) {
    // check file tab capacity
    if(opened_file_info->cnt >= MAX_FILE_TAB_CNT) {
        // todo : check saved & its modified time, close oldest. if failed, return -1
        perror("file open cap over");
        return -1;
    }

    FileStatus *node = malloc(sizeof(FileStatus));
    strcpy(node->file_name, file_name);
    strcpy(node->full_path, full_path);
    node->row = node->col = node->modified = node->start_row = node->start_col = 0;
    node->last_saved = time(NULL);

    // update opened_file_info
    opened_file_info->focus = node;
    if(opened_file_info->cnt == 0) {
        opened_file_info->focus_strlen = strlen(file_name);
    }
    node->next = opened_file_info->head;
    opened_file_info->head = node;
    opened_file_info->cnt++;

    // open file
    node->fd = open(full_path, O_RDONLY);
    if(node->fd != -1) {
        node->buf_cnt = get_file_size(node->fd) / BUFSIZ + 1;
        // initialize CodeBuf
        code_buf_initialize(node);
    }

    return 0;
}

void del_opened_file_tab(int idx) {
    int focus_flag = 0;
    FileStatus *cur = opened_file_info->head;
    FileStatus *pre = NULL;
    if(cur == NULL) {
        perror("del_opened_file_tab : out of index");
        return;
    }

    for(int i = 0; i < idx; i++) {
        if(cur == NULL) {
            perror("del_opened_file_tab : out of index");
            return;
        }
        pre = cur;
        cur = cur->next;
    }

    // check file is saved
    if(cur->modified != 0) {
        close_unsaved_caution(idx);
        return;
    }

    // focus check before delete
    if(cur == opened_file_info->focus) {
        focus_flag = 1;
    }

    // delete file tab
    if(pre == NULL) { // del head
        opened_file_info->head = cur->next;
    } else {
        pre->next = cur->next;
    }
    file_status_close(cur);
    opened_file_info->cnt--;

    // focus update if needed
    if(focus_flag == 1) {
        if(opened_file_info->cnt > 0) {
            opened_file_info->focus = opened_file_info->head;
            opened_file_info->focus_strlen = strlen(opened_file_info->head->file_name);
        } else {
            opened_file_info->focus = NULL;
        }
    }
    opened_file_tab_print();
    code_contents_print();
}

OpenFileInfo *opened_file_info_init() {
    OpenFileInfo *info = malloc(sizeof(OpenFileInfo));
    info->cnt = 0;
    info->focus = NULL;
    info->head = NULL;

    return info;
}

void opened_file_info_terminate() {
    FileStatus *ptr = opened_file_info->head;
    if(ptr != NULL) {
        FileStatus *next;
        while(ptr != NULL) {
            next = ptr->next;
            file_status_close(ptr);
            ptr = next;
        }
    }
    free(opened_file_info);
}

int opened_file_focus_idx_find() {
    int idx = 0;
    FileStatus *ptr = opened_file_info->head;
    while(1) {
        if(ptr == NULL) {
            return -1;
        }
        if(ptr == opened_file_info->focus) {
            return idx;
        }
        ptr = ptr->next;
        idx++;
    }
}

void opened_file_focus_next() {
    if(opened_file_info->cnt < 2)
        return;
    
    FileStatus *ptr = opened_file_info->focus->next;
    if(ptr == NULL)
        ptr = opened_file_info->head;
    
    opened_file_info->focus = ptr;
}

void opened_file_focus_prev() {
    if(opened_file_info->cnt < 2)
        return;

    FileStatus *cur = opened_file_info->head;
    FileStatus *pre = NULL;

    while(1) {
        if(cur == NULL)
            return;
        if(cur == opened_file_info->focus)
            break;
        pre = cur;
        cur = cur->next;
    }

    if(pre == NULL) {
        while (cur != NULL)
        {
            pre = cur;
            cur = cur->next;
        }
        
    }
    opened_file_info->focus = pre;
}

void close_unsaved_caution(int idx) {
    unsaved_caution_flag = idx + 1; // change input handle procedure
    // print caution msg
    wclear(contents);
    int row = (win_row - 3) / 2;
    int col = win_col / 2;

    mvwaddstr(contents, row - 2, col - 18, "You are trying to close Unsaved File");
    mvwaddch(contents, row - 1, col - 8, '[');
    wattron(contents, A_UNDERLINE);
    mvwaddch(contents, row - 1, col - 7, 'S');
    wattroff(contents, A_UNDERLINE);
    mvwaddstr(contents, row - 1, col - 6, "ave and close]");
    mvwaddch(contents, row, col - 10, '[');
    wattron(contents, A_UNDERLINE);
    mvwaddch(contents, row, col - 9, 'C');
    wattroff(contents, A_UNDERLINE);
    mvwaddstr(contents, row, col - 8, "lose without save]");
    mvwaddstr(contents, row + 1, col - 11, "[input else to cancel]");

    wrefresh(contents);
}

off_t get_file_size(int fd) {
    struct stat st;
    if(fstat(fd, &st) == -1) {
        perror("get_file_size : failed to stat");
        return 0;
    }

    return st.st_size;
}