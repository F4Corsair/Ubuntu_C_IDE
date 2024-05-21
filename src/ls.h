#ifndef LS_H
#define LS_H

#include <ncurses.h>
#include "global.h"

void displayDirectoryContents(WINDOW *contents_win, char *path);

#endif /* LS_H */