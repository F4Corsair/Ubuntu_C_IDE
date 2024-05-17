#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "global.h"
#include "code.h"
#include "layout.h"
#include "winsize.h"

void file_open_update();
FileStatus file_open(char *file_name);