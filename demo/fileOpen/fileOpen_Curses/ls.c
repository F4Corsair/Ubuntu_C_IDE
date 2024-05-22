#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>
#include "ls.h"

int row;
int col;

void displayDirectoryContents(WINDOW *contents_win, char *path);

void displayDirectoryContentsRecursive(WINDOW *contents_win, char *path) {
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

        struct stat info;
        if (stat(full_path, &info) == -1)
            continue;
        if (S_ISDIR(info.st_mode)) {
            wattron(contents_win, COLOR_PAIR(2));
            mvwprintw(contents_win, row, col, "v ");
            wattroff(contents_win, COLOR_PAIR(2)); 
            mvwprintw(contents_win, row++, col+2, "%s", direntp->d_name);
           
            col += 2;
            displayDirectoryContentsRecursive(contents_win, full_path);
            col = 1;
        } else {
            if (col == 1)
                mvwprintw(contents_win, row++, col, "%s", direntp->d_name);
            else{
                wattron(contents_win, COLOR_PAIR(3));
                mvwprintw(contents_win, row, col, "> ");
                wattroff(contents_win, COLOR_PAIR(3)); 
                mvwprintw(contents_win, row++, col+2, "%s", direntp->d_name);
            }
        }
    }

    closedir(dir_ptr);
}

void displayDirectoryContents(WINDOW *contents_win, char *path) {
    row = 1;
    col = 1;
    start_color();
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    displayDirectoryContentsRecursive(contents_win, path);
    wrefresh(contents_win);
}
