// all golbal variables & struct usually shared or play critical role between codes shouled defined at here
#ifndef __GlobalHeader
#define __GlobalHeader

#include <curses.h>
#include <time.h>

/* STRUCT */

typedef struct _code_line { // doubly linked list
    char *line;
    int len;
    struct _code_line *prev, *next;
} CodeLine;

typedef struct _code_buf {
    int tail_row;
    CodeLine *head, *tail, *cur;
    int end_with_new_line;
} CodeBuf;

typedef struct _file_status{
	char file_name[256];
	char full_path[256];
    int fd;
    int buf_cnt;
    int row, col;
    int start_row, start_col; // CodeBuf->cur <= start_row
    int modified; // 0 : unmodified  1 : tmp saved  2 : modified
    time_t last_saved;
    CodeBuf *buf;
    struct _file_status *next;
} FileStatus; // saves opened file status - uses at file open & menu tab print ...

typedef struct {
    FileStatus *head;
    int cnt;
    FileStatus *focus;
} OpenFileInfo;

enum MenuTab {
    CODE_TAB,
    FILE_TAB,
    BUILD_TAB,
    TERMINAL_TAB,
    MANUAL_TAB,
    QUIT_TAB,
    WINSIZE_TAB
};

enum ManualPage {
    INTRO_MAN,
    CODE_MAN,
    FILE_MAN,
    BUILD_MAN
};

/* GLOBAL VARIABLES */

// global.c
extern int winsize_flag; // 0 : default  1 : blocked  2 : ready to refresh

extern enum MenuTab menu_tab_focus; // will define its value at menuTab.h
extern enum MenuTab menu_tab_focus_backup[2];

extern int win_row;
extern int win_col;

// uibase.c
extern OpenFileInfo *opened_file_info;

extern WINDOW *opened_file_tab;
extern WINDOW *menu_tab;
extern WINDOW *contents;

//fileTab.c
extern FileStatus* head;

// openedFileTab.c
extern int unsaved_caution_flag;

// manual.c
extern enum ManualPage manual_page_focus;

extern int workspace_contents_row;
extern int workspace_contents_col;
#endif

/* MACRO FUNCTIONS */
#ifndef CTRL
#define CTRL(c) ((c) & 037)// macro function to read CTRL + character
#endif