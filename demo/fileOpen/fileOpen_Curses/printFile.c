#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "global.h"

#define COMMENT_NO 0
#define COMMENT_SINGLE 1
#define COMMENT_MULTI 2

FileStatus fileOpen(char *file_name);

void printFile_win(char *file_name) {
	int ch, prev, row, col;
	prev = EOF;
	int x, y;
	int state = COMMENT_NO;
	FileStatus file1;
	file1 = fileOpen(file_name);

	FILE *fp = fopen(file1.file_name, "r");
	if (fp == NULL) {
		perror("Cannot open file");
		exit(1);
	}

	initscr();
	if(has_colors() == FALSE) {
		endwin();
		printf("Your terminal does not support color\n");
		exit(1);
	}
	start_color();
	init_pair(2, COLOR_GREEN, COLOR_BLACK); // for comments

	getmaxyx(stdscr, row, col);
	
	while((ch = fgetc(fp)) != EOF) {
		getyx(stdscr, y, x); // get cursor position
		
		if(y == (row - 1)) {
			printw("file path : %s", file1.full_path);
			getch();
			clear();
			move(0, 0);
		}

		if(prev == '/' && state == COMMENT_NO) // comment
		{
			if (ch == '*') {
				attron(A_BOLD);
				attron(COLOR_PAIR(2));
				state = COMMENT_MULTI;
				getyx(stdscr, y, x);
				move(y, x - 1);
				printw("%c%c", '/', ch);
			}
			else if (ch == '/') {
				attron(A_BOLD);
				attron(COLOR_PAIR(2));
				state = COMMENT_SINGLE;
				getyx(stdscr, y, x);
				move(y, x - 1);
				printw("%c%c", '/', ch);
			}
		}
		else
			printw("%c", ch);

		refresh();

		if(state == COMMENT_MULTI && (prev == '*' && ch == '/')) {
			attroff(A_BOLD);
			attroff(COLOR_PAIR(2));
			state = COMMENT_NO;
		}
		else if(state == COMMENT_SINGLE && ch == '\n') {
			attroff(A_BOLD);
			attroff(COLOR_PAIR(2));
			state = COMMENT_NO;
		}

		prev = ch;
	}

	mvprintw(row - 1, 0, "file path : %s", file1.full_path);
	refresh();
	getch();

	endwin();
	fclose(fp);
}


/*
 * comment
 * for
 * test
 */
