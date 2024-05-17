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

    menu_tab_focus = CODE_TAB;
    menu_tab_update();
    wrefresh(menu_tab);

    // todo : show CODE_TAB
    opened_file_tab_print();
    wrefresh(opened_file_tab);
}

/* opened_file_tab */

void opened_file_tab_print() {
    if(opened_file_info->cnt > MAX_FILE_TAB_CNT) {
        perror("opend_file_tab out of index");
        return;
    }

    wclear(opened_file_tab);
    wrefresh(opened_file_tab);

    wattron(opened_file_tab, A_UNDERLINE);
    mvwaddstr(opened_file_tab, 0, 0, "File Focus");
    wattroff(opened_file_tab, A_UNDERLINE);
    mvwaddch(opened_file_tab, 0, INFO_LABEL_WIDTH - 1, '/');

    int col = INFO_LABEL_WIDTH;
    int focus_len = FILE_TAB_FOCUS_MAX_WIDTH - 2;
    int len;
    if(opened_file_info->cnt != 0)
        len = (win_col - INFO_LABEL_WIDTH - focus_len) / opened_file_info->cnt;
    FileStatus *ptr = opened_file_info->head;
    for(int i = 0; i < opened_file_info->cnt; i++) {
        mvwaddch(opened_file_tab, 0, col++, '\\');
        wattron(opened_file_tab, A_UNDERLINE);
        if(ptr == opened_file_info->focus) {
            wattron(opened_file_tab, A_STANDOUT);
            // mvwaddstr(opened_file_tab, 0, col, "");
            move(0, col);
            wprintw(opened_file_tab, "%-*s", focus_len, ptr->file_name);
            col = col + focus_len;
            wattroff(opened_file_tab, A_STANDOUT);
        } else {
            move(0, col);
            wprintw(opened_file_tab, "%-*s", len, ptr->file_name);
            col = col + len;
        }
        wattroff(opened_file_tab, A_UNDERLINE);
        mvwaddch(opened_file_tab, 0, col++, '/');
        ptr = ptr->next;
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
    node->row = node->col = node->modified = 0;
    node->last_saved = time(NULL);

    // update opened_file_info
    if(opened_file_info->cnt == 0) {
        opened_file_info->focus = node;
        opened_file_info->focus_strlen = strlen(file_name);
    }
    node->next = opened_file_info->head;
    opened_file_info->head = node;
    opened_file_info->cnt++;

    // open file
    node->fd = open(full_path, O_RDONLY);
    // todo : if fd == -1, show error msg at code viewer

    return 0;
}

void del_opened_file_tab(int idx) {
    FileStatus *cur = opened_file_info->head;
    FileStatus *pre = NULL;
    for(int i = idx; i > 0; i--) {
        if(cur == NULL) {
            perror("del_opened_file_tab : out of index");
            return;
        }
        pre = cur;
        cur = cur->next;
    }

    // check file is saved
    if(cur->modified != 0) {
        if(close_unsaved_caution() == -1)
            return;
    }

    // delete file tab
    if(pre == NULL) { // del head
        opened_file_info->head = cur->next;
    } else {
        pre->next = cur->next;
    }
    if(cur->fd != -1)
        close(cur->fd);
    free(cur);
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
            free(ptr);
            ptr = next;
        }
    }
    free(opened_file_info);
}

void opened_file_focus_next() {

}

void opened_file_focus_prev() {

}

int close_unsaved_caution() {
    // ret -1 : don't save & cancel close process
}