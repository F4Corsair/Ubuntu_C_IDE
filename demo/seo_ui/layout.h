#ifndef LAYOUT_H
#define LAYOUT_H

#define CTRL(c) ((c) & 037)  // macro function to read CTRL + character
#define DEBUG_INPUT // debug option : print input character

#define KEY_NAME_WIDTH 14
#define FILE_TAB_WIDTH 15
#define MIN_MENU_TAB_WIDTH 10
#define MENU_TAB_CNT 6
#define MIN_WIDTH (MIN_MENU_TAB_WIDTH * MENU_TAB_CNT)

#define CODE_TAB 0
#define FILE_TAB 1
#define BUILD_TAB 2
#define TERMINAL_TAB 3
#define MANUAL_TAB 4
#define QUIT_TAB 5


typedef struct {
    char *file_name;
    char *full_path;
} TabContents;

char menu_tab_names[MENU_TAB_CNT][MIN_MENU_TAB_WIDTH - 1] = {"  Code  ", "  File  ", "  Build ", "Terminal", " Manual ", "  Quit  "};
int menu_tab_pos[MENU_TAB_CNT] = {2, 2, 2, 0, 1, 2}; 

void newFileTab(WINDOW *file_tab);
void printPath(WINDOW *path_win, char *path);
void menuTabPrint(WINDOW *menu_tab, int max_col, int focus);


WINDOW *file_contents;
WINDOW *file_path;
WINDOW *file_tab;
WINDOW *file_menu_tab;



#endif /* LATOUT_H */
