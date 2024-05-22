#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>

#include "fileSave.h"

void file_save_focus() {
    _file_save(opened_file_info->focus);
}

void file_save_idx(int idx) {
    FileStatus *ptr = opened_file_info->head;
    for(; idx > 0; idx--) {
        if(ptr != NULL) {
            ptr = ptr->next;
        } else {
            perror("file_save_idx() : ptr out of idx");
            return;
        }
    }
    if(ptr == NULL) {
            perror("file_save_idx() : ptr out of idx");
            return;
    }
    _file_save(ptr);
}

void _file_save(FileStatus *status) {
    if(status->modified == 0) {
        return;
    }

    int fd = open(status->full_path, O_WRONLY | O_TRUNC);
    if(fd < 0) {
        perror("_file_save() : Can't open file");
        fprintf(stderr, "file name : %s\n", status->full_path);
        return;
    }

    // lock fd with flock (LOCK_EX) to prevent race condition
    if(flock(fd, LOCK_EX) < 0) {
        perror("_file_save() : flock");
        close(fd);
        return;
    }

    // save file - overwrite original file
    CodeLine *ptr = status->buf->head;

    if(ptr != NULL) {
        while (1) {
            write(fd, ptr->line, ptr->len);
            if(ptr->next == NULL)
                break;
            write(fd, "\n", 1);
            ptr = ptr->next;
        }
    }

    // make file unmodified
    status->modified = 0;

    // open lock & close
    flock(fd, LOCK_UN);
    close(fd);
}