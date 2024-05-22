#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

void display_file(WINDOW *win, FILE *file) {
    char line[COLS];
    int y, x;

    while (fgets(line, sizeof(line), file) != NULL) {
       	getyx(win, y, x); 
        if (y == LINES - 1) {
            mvprintw(LINES - 1, 0, "<- Press any key to continue ->");
            refresh();
            getch(); 
            clear(); 
            refresh();
            y = 0; 
	}

        mvwprintw(win, y, 0, "%s", line);
        wrefresh(win);
    }
}


int main() {
    FILE *file;
    int ch;

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);

    file = fopen("controlflow.c", "r");
    if (file == NULL) {
        printw("Error opening file.\n");
        refresh();
        getch(); 
        endwin();
        exit(EXIT_FAILURE);
    }

    // todo : 새 윈도우 생성해서 표시
    WINDOW *win = newwin(LINES - 1, COLS, 0, 0);

    // 파일 표시
    display_file(stdscr, file);

    while ((ch = getch()) != KEY_F(1)) { // F1 = exit
        switch (ch) {
            case KEY_DOWN:
                scroll(stdscr); // 아래로 스크롤
                break;
            case KEY_UP:
                scrl(-1); // 위로 스크롤
                break;
        }
        refresh();
    }

    fclose(file);
    endwin();

    return 0;
}

