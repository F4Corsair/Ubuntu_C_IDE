#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include <stdlib.h>
#include <stddef.h>
#include "ls.h"
#include "global.h"

int contents_row;
int contents_col;

FileStatus* head;

void lsR(WINDOW *contents_win, char *path);
void displayDirectoryContents(WINDOW *contents_win, char *path);
void addToList(char *file_name, char *full_path);



void lsR(WINDOW *contents_win, char *path) {
    DIR *dir_ptr;
    struct dirent *direntp;

    if ((dir_ptr = opendir(path)) == NULL) {
        fprintf(stderr, "ls2: cannot open %s\n", path);
        return;
    }

    while ((direntp = readdir(dir_ptr)) != NULL) {
        if (strcmp(direntp->d_name, ".") == 0 || strcmp(direntp->d_name, "..") == 0)
            continue;

        char full_path[PATH_MAX];
        snprintf(full_path, PATH_MAX, "%s/%s", path, direntp->d_name);
        
        addToList(direntp->d_name,full_path);
        
        struct stat info;
        if (stat(full_path, &info) == -1)
            continue;
        if (S_ISDIR(info.st_mode)) {
            wattron(contents_win, COLOR_PAIR(2));
            mvwprintw(contents_win, contents_row,contents_col, "v ");
            wattroff(contents_win, COLOR_PAIR(2)); 
            mvwprintw(contents_win, contents_row++, contents_col+2, "%s", direntp->d_name);
            
            contents_col += 2;
            lsR(contents_win, full_path);
            contents_col = 1;
        } else {
            if (contents_col == 1)
                mvwprintw(contents_win, contents_row++, contents_col, "%s", direntp->d_name);
            else{
                wattron(contents_win, COLOR_PAIR(3));
                mvwprintw(contents_win, contents_row, contents_col, "> ");
                wattroff(contents_win, COLOR_PAIR(3)); 
                mvwprintw(contents_win, contents_row++, contents_col+2, "%s", direntp->d_name);
            }
        }

        /**/
        
    }

    closedir(dir_ptr);
}

void ls(WINDOW *contents_win, char *path) {
    contents_col = 1;
    contents_row = 1;

    head=NULL;

    start_color();
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    lsR(contents_win, path);
    FileStatus *cur;
        cur=head;
        for(;cur;cur=cur->next){
            mvwprintw(contents_win,contents_row++,contents_col,"%s %s",cur->file_name,cur->full_path);
        }
    wrefresh(contents_win);
}

void addToList(char *file_name, char *full_path) {
    FileStatus *new_node = (FileStatus*) malloc(sizeof(FileStatus));
   
    if (!new_node) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    strcpy(new_node->file_name,file_name);
    //new_node->file_name = strdu(file_name);
    strcpy(new_node->full_path,full_path);
    //new_node->full_path = strdup(full_path);
    //new_node->position = contents_row; // Store the position if necessary
    new_node->next = NULL;

    if (!head) {
        head = new_node;
    } else {
        FileStatus *current = head;
        while (current->next) {
            current = current->next;
        }
        current->next = new_node;
    }
}