#include <curses.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

#include "global.h"
#include "file.h"
#include "uibase.h"
#include "code.h"
#include "winsize.h"
#include "openedFileTab.h"

int contents_row;
int contents_col;
int workspace_contents_row;
int workspace_contents_col;
int workspace_file_focus; // contents win 에서 첫번째 나오는 파일이름이 연결리스트에서 몇번째 파일인지
int workspace_file_finish;
int workspace_flag;
int directory_check;
int file_check;

WorkSpaceFile *workspace_directory;
WorkSpaceFile *contents_head;
WorkSpaceFile *filetab_head;

void file_tab_transition() {
    if(menu_tab_focus == FILE_TAB)
        return;

    erase();
    refresh();

    menu_tab_focus = FILE_TAB;
    menu_tab_update();
    wrefresh(menu_tab);

    // show FILE_TAB
    opened_workspace_tab_print("/home");
    workspace_contents_print();
}

void file_tab_init()
{
    workspace_flag=0;
    start_color();
}

void file_open_update(int *input_char)
{
    winsize_calculate();
    wclear(contents);

    int row_pos = win_row / 2 - 1;
    mvwaddstr(contents, row_pos++, win_col / 2 - 11, "Code file tab is full!");
    mvwaddstr(contents, row_pos++, win_col / 2 - 19, "Please close opened file from code tab");
    wrefresh(contents);

    *(input_char) = getch();
}

void contents_window_restore()
{
    wclear(contents);

    if (menu_tab_focus == CODE_TAB)
    {
        opened_file_tab_print();
        code_contents_print();
    }
    else if (menu_tab_focus == FILE_TAB)
    {
        opened_workspace_tab_print();
        workspace_contents_print();
    }
}

void file_open(char *file_name, char *full_path) {
	char path[256];
	char subdirectory[256];
	char *ptr;
	int flag, input_char;

	if (getcwd(path, 256) == NULL) {
		perror("getcwd");
		exit(1);
	}
	strcat(path, "/");
	strcat(path, file_name);

	if (strcmp(path, full_path) != 0) {
		strcpy(subdirectory, full_path);
		ptr = strrchr(subdirectory, '/');
		ptr = '\0';

		if (access(full_path, F_OK) != 0) {
            perror("file does not exist");
            exit(1);
		}

		flag = new_opened_file_tab(file_name, subdirectory);		
		if (flag == -1) {
            file_open_update(&input_char);
            contents_window_restore();
        }
	}
	else {
		// file existence check
        if (access(path, F_OK) != 0) {
        	perror("file does not exist");
        	exit(1);
   	 	}

		flag = new_opened_file_tab(file_name, path);
		if (flag == -1) {	
            file_open_update(&input_char);
            contents_window_restore();
        }
	}
}

void print_path(const char *path)
{
    int width = win_col - 2;
    werase(opened_file_tab);
    wattron(opened_file_tab, A_UNDERLINE);
    mvwprintw(opened_file_tab, 0, 0, "Path:");
    mvwprintw(opened_file_tab, 0, 5, " %-*s", width, path);
    wattroff(opened_file_tab, A_UNDERLINE);
    wrefresh(opened_file_tab);
}

void opened_workspace_tab_print()
{
    char path[256];

    if (getcwd(path, sizeof(path)) != NULL)
        print_path(path);
    wrefresh(opened_file_tab);
}

void addToList(WorkSpaceFile **head, char *file_name, char *full_path)
{
    WorkSpaceFile *new_node = (WorkSpaceFile *)malloc(sizeof(WorkSpaceFile));

    if (!new_node)
    {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    strcpy(new_node->file_name, file_name);
    strcpy(new_node->full_path, full_path);
    new_node->row = contents_row;
    new_node->col = contents_col;
    new_node->next = NULL;
    struct stat info;
    stat(full_path, &info);
    if (S_ISDIR(info.st_mode))
    {
        new_node->position = 1;
    }
    else
    {
        if (new_node->col > 1)
            new_node->position = 2;
        else
            new_node->position = 0;
    }
    if (!*head)
    {
        *head = new_node;
    }
    else
    {
        WorkSpaceFile *current = *head;
        while (current->next)
        {
            current = current->next;
        }
        current->next = new_node;
    }
}

void lsR(char *path)
{
    DIR *dir_ptr;
    struct dirent *direntp;

    if ((dir_ptr = opendir(path)) == NULL)
    {
        fprintf(stderr, "ls2: cannot open %s\n", path);
        return;
    }

    while ((direntp = readdir(dir_ptr)) != NULL)
    {
        if (strcmp(direntp->d_name, ".") == 0 || strcmp(direntp->d_name, "..") == 0)
            continue;

        char full_path[512];
        sprintf(full_path, "%s/%s", path, direntp->d_name);

        struct stat info;
        if (stat(full_path, &info) == -1)
            continue;
        file_check++;
        // 리스트에 추가
        addToList(&contents_head, direntp->d_name, full_path);
        if (contents_row <= win_row - 4)
        {
            if (S_ISDIR(info.st_mode))
            {
                wattron(contents, COLOR_PAIR(3));
                mvwprintw(contents, contents_row, contents_col, "v ");
                wattroff(contents, COLOR_PAIR(3));
                mvwprintw(contents, contents_row++, contents_col + 2, "%s", direntp->d_name);
            }
            else
            {
                if (contents_col == 1)
                    mvwprintw(contents, contents_row++, contents_col, "%s", direntp->d_name);
                else
                {
                    wattron(contents, COLOR_PAIR(4));
                    mvwprintw(contents, contents_row, contents_col, "> ");
                    wattroff(contents, COLOR_PAIR(4));
                    mvwprintw(contents, contents_row++, contents_col + 2, "%s", direntp->d_name);
                }
            }
        }

        if (S_ISDIR(info.st_mode))
        {
            contents_col += 2;
            lsR(full_path);
            contents_col -= 2;
        }
    }

    closedir(dir_ptr);
}

void ls(char *path)
{
    contents_col = 1;
    contents_row = 0;
    file_check = 0;
    contents_head = NULL;

    werase(contents);
    lsR(path);
    wrefresh(contents);
}

void ls_directory(char *path)
{
    DIR *dir_ptr;
    struct dirent *direntp;
    directory_check = 0;

    if ((dir_ptr = opendir(path)) == NULL)
    {
        fprintf(stderr, "ls: cannot open %s\n", path);
        return;
    }

    contents_col = 1;
    contents_row = 0;
    contents_head = NULL;
    werase(contents);

    while ((direntp = readdir(dir_ptr)) != NULL)
    {
        if (strcmp(direntp->d_name, ".") == 0 || strcmp(direntp->d_name, "..") == 0)
            continue;

        char full_path[512];
        sprintf(full_path, "%s/%s", path, direntp->d_name);

        struct stat info;
        if (stat(full_path, &info) == -1)
            continue;

        if (S_ISDIR(info.st_mode))
        {
            addToList(&contents_head, direntp->d_name, full_path);
            directory_check = 1;
            if (contents_row <= win_row - 4)
            {
                mvwprintw(contents, contents_row++, contents_col, "%s", direntp->d_name);
            }
        }
    }

    closedir(dir_ptr);
    wrefresh(contents);
}

void workspace_contents_print()
{
    char path[256];
    workspace_contents_row = 0;
    workspace_contents_col = 1;
    workspace_file_focus = 0;
    workspace_file_finish = 0;
    contents_head = NULL;

    char *message1 = "There is no sub-directory in here.";
    char *message2 = "Please press the left arrow key to go back.";
    int msg1_len = strlen(message1);
    int msg2_len = strlen(message2);

    if (getcwd(path, sizeof(path)) != NULL)
    {
        if (workspace_flag == 0)
        {
            ls_directory(path);
            if (directory_check == 1)
            {
                wmove(contents, workspace_contents_row, 0);
                wclrtoeol(contents);
                mvwprintw(contents, workspace_contents_row, workspace_contents_col, "%s", contents_head->file_name);
                mvwchgat(contents, workspace_contents_row, workspace_contents_col, strlen(contents_head->file_name), A_STANDOUT, 0, NULL);
            }
            else
            {
                wattron(contents, COLOR_PAIR(3));

                wclear(contents);
                mvwprintw(contents, (win_row - 3) / 2, (win_col - msg1_len) / 2, "%s", message1);
                mvwprintw(contents, ((win_row - 3) / 2) + 1, (win_col - msg2_len) / 2, "%s", message2);
                wrefresh(contents);
                wattroff(contents, COLOR_PAIR(3));
            }
        }
        else
        {
            ls(path);
            if (file_check > 0)
            {
                wmove(contents, workspace_contents_row, 0);
                wclrtoeol(contents);
                mvwprintw(contents, workspace_contents_row, workspace_contents_col, "%s", contents_head->file_name);
                mvwchgat(contents, workspace_contents_row, workspace_contents_col, strlen(contents_head->file_name), A_STANDOUT, 0, NULL);
            }
            else
            {
                workspace_flag = 0;
                wattron(contents, COLOR_PAIR(3));

                wclear(contents);
                mvwprintw(contents, (win_row - 3) / 2, (win_col - msg1_len) / 2, "%s", message1);
                mvwprintw(contents, ((win_row - 3) / 2) + 1, (win_col - msg2_len) / 2, "%s", message2);
                wrefresh(contents);
                wattroff(contents, COLOR_PAIR(3));
            }
        }
    }
    wrefresh(contents);
}

void workspace_key_up()
{
    int workspace_key_col;
    if (workspace_contents_row == 0 && workspace_file_focus > 0)
    {
        // 화면의 첫 줄에 도달하고, 이전 파일이 존재하는 경우
        if (workspace_file_finish == -1)
        {
            workspace_file_finish = 0;
        }
        workspace_file_focus--;
        WorkSpaceFile *cur = contents_head;
        for (int i = 0; i < workspace_file_focus; i++)
        {
            cur = cur->next;
        }
        werase(contents); // 윈도우 지우기
        for (int row = 0; row < win_row - 3; row++)
        {
            if (cur != NULL)
            {
                if (workspace_flag == 0)
                {
                    mvwprintw(contents, row, cur->col, "%s", cur->file_name);
                }
                else
                {
                    switch (cur->position)
                    {
                    case 0:
                        mvwprintw(contents, row, cur->col, "%s", cur->file_name);
                        workspace_key_col = cur->col;
                        break;
                    case 1:
                        wattron(contents, COLOR_PAIR(3));
                        mvwprintw(contents, row, cur->col, "%s", "v ");
                        wattroff(contents, COLOR_PAIR(3));
                        mvwprintw(contents, row, cur->col + 2, "%s", cur->file_name);
                        workspace_key_col = cur->col + 2;
                        break;
                    default:
                        wattron(contents, COLOR_PAIR(4));
                        mvwprintw(contents, row, cur->col, "%s", "> ");
                        wattroff(contents, COLOR_PAIR(4));
                        mvwprintw(contents, row, cur->col + 2, "%s", cur->file_name);
                        workspace_key_col = cur->col + 2;
                        break;
                    }
                }

                cur = cur->next;
            }
            else
            {
                break;
            }
        }
        wrefresh(contents);
        cur = contents_head;
        for (int i = 0; i < workspace_file_focus; i++)
        {
            cur = cur->next;
        }
        if (workspace_flag == 0)
            mvwchgat(contents, 0, cur->col, strlen(cur->file_name), A_STANDOUT, 0, NULL);
        else
            mvwchgat(contents, 0, workspace_key_col, strlen(cur->file_name), A_STANDOUT, 0, NULL);
    }
    else if (workspace_contents_row > 0)
    {
        // 화면 내에서 이동
        if (workspace_file_finish == -1)
        {
            workspace_file_finish = 0;
        }
        wchgat(contents, -1, A_NORMAL, 0, NULL);
        workspace_contents_row--;
        WorkSpaceFile *cur = contents_head;
        for (int i = 0; i < workspace_contents_row + workspace_file_focus; i++)
        {
            cur = cur->next;
        }

        if (cur != NULL)
        {
            wmove(contents, workspace_contents_row, 0);
            wclrtoeol(contents);
            if (workspace_flag == 0)
            {
                mvwprintw(contents, workspace_contents_row, cur->col, "%s", cur->file_name);
            }
            else
            {
                switch (cur->position)
                {
                case 0:
                    mvwprintw(contents, workspace_contents_row, cur->col, "%s", cur->file_name);
                    workspace_key_col = cur->col;
                    break;
                case 1:
                    wattron(contents, COLOR_PAIR(3));
                    mvwprintw(contents, workspace_contents_row, cur->col, "%s", "v ");
                    wattroff(contents, COLOR_PAIR(3));
                    mvwprintw(contents, workspace_contents_row, cur->col + 2, "%s", cur->file_name);
                    workspace_key_col = cur->col + 2;
                    break;
                default:
                    wattron(contents, COLOR_PAIR(4));
                    mvwprintw(contents, workspace_contents_row, cur->col, "%s", "> ");
                    wattroff(contents, COLOR_PAIR(4));
                    mvwprintw(contents, workspace_contents_row, cur->col + 2, "%s", cur->file_name);
                    workspace_key_col = cur->col + 2;
                    break;
                }
            }
            if (workspace_flag == 0)
                mvwchgat(contents, workspace_contents_row, cur->col, strlen(cur->file_name), A_STANDOUT, 0, NULL);
            else
                mvwchgat(contents, workspace_contents_row, workspace_key_col, strlen(cur->file_name), A_STANDOUT, 0, NULL);
            wrefresh(contents);
        }
    }
}

void workspace_key_down()
{
    int workspace_key_col;
    if (workspace_contents_row == win_row - 4 && workspace_file_finish != -1)
    {
        workspace_file_focus++;
        WorkSpaceFile *cur = contents_head;
        for (int i = 0; i < workspace_file_focus; i++)
        {
            if (cur == NULL)
            {
                workspace_file_finish = -1;
                break;
            }
            cur = cur->next;
        }
        if (cur != NULL)
        {
            werase(contents);
            for (int row = 0; row < win_row - 3; row++)
            {
                if (cur != NULL)
                {
                    if (workspace_flag == 0)
                    {
                        mvwprintw(contents, row, cur->col, "%s", cur->file_name);
                    }
                    else
                    {
                        switch (cur->position)
                        {
                        case 0:
                            mvwprintw(contents, row, cur->col, "%s", cur->file_name);
                            workspace_key_col = cur->col;
                            break;
                        case 1:
                            wattron(contents, COLOR_PAIR(3));
                            mvwprintw(contents, row, cur->col, "%s", "v ");
                            wattroff(contents, COLOR_PAIR(3));
                            mvwprintw(contents, row, cur->col + 2, "%s", cur->file_name);
                            workspace_key_col = cur->col + 2;
                            break;
                        default:
                            wattron(contents, COLOR_PAIR(4));
                            mvwprintw(contents, row, cur->col, "%s", "> ");
                            wattroff(contents, COLOR_PAIR(4));
                            mvwprintw(contents, row, cur->col + 2, "%s", cur->file_name);
                            workspace_key_col = cur->col + 2;
                            break;
                        }
                    }
                    if (row < win_row - 2)
                        cur = cur->next;
                }
                else
                {
                    workspace_file_finish = -1;
                    break;
                }
            }
            if (cur != NULL)
            {
                cur = contents_head;
                for (int i = 0; i < workspace_contents_row + workspace_file_focus; i++)
                {
                    cur = cur->next;
                }
                if (workspace_flag == 0)
                    mvwchgat(contents, win_row - 4, cur->col, strlen(cur->file_name), A_STANDOUT, 0, NULL);
                else
                    mvwchgat(contents, win_row - 4, workspace_key_col, strlen(cur->file_name), A_STANDOUT, 0, NULL);
            }
        }
        else
        {
            workspace_file_focus--;
        }
        wrefresh(contents);
    }
    else if (workspace_contents_row < win_row - 4 && workspace_file_finish != -1 && workspace_contents_row < num_files_to_display() - 1)
    {
        WorkSpaceFile *cur = contents_head;
        for (int i = 0; i < workspace_contents_row + workspace_file_focus; i++)
        {
            cur = cur->next;
        }
        if (cur != NULL && cur->next != NULL)
        {
            wchgat(contents, -1, A_NORMAL, 0, NULL);
            workspace_contents_row++;
            cur = contents_head;
            for (int i = 0; i < workspace_contents_row + workspace_file_focus; i++)
            {
                cur = cur->next;
            }
            if (workspace_flag == 0)
            {
                mvwprintw(contents, workspace_contents_row, cur->col, "%s", cur->file_name);
            }
            else
            {
                switch (cur->position)
                {
                case 0:
                    mvwprintw(contents, workspace_contents_row, cur->col, "%s", cur->file_name);
                    workspace_key_col = cur->col;
                    break;
                case 1:
                    wattron(contents, COLOR_PAIR(3));
                    mvwprintw(contents, workspace_contents_row, cur->col, "%s", "v ");
                    wattroff(contents, COLOR_PAIR(3));
                    mvwprintw(contents, workspace_contents_row, cur->col + 2, "%s", cur->file_name);
                    workspace_key_col = cur->col + 2;
                    break;
                default:
                    wattron(contents, COLOR_PAIR(4));
                    mvwprintw(contents, workspace_contents_row, cur->col, "%s", "> ");
                    wattroff(contents, COLOR_PAIR(4));
                    mvwprintw(contents, workspace_contents_row, cur->col + 2, "%s", cur->file_name);
                    workspace_key_col = cur->col + 2;
                    break;
                }
            }
            if (workspace_flag == 0)
                mvwchgat(contents, workspace_contents_row, cur->col, strlen(cur->file_name), A_STANDOUT, 0, NULL);
            else
                mvwchgat(contents, workspace_contents_row, workspace_key_col, strlen(cur->file_name), A_STANDOUT, 0, NULL);
        }
        else
        {
            workspace_file_finish = -1;
        }
        wrefresh(contents);
    }
}

int num_files_to_display()
{
    int count = 0;
    WorkSpaceFile *cur = contents_head;
    while (cur != NULL && count < win_row - 3)
    {
        count++;
        cur = cur->next;
    }
    return count;
}

void workspacefile_terminate(WorkSpaceFile *head)
{
    WorkSpaceFile *temp;
    while (head != NULL)
    {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void new_file_open() {
    int fd;
    char file_name[256];
    char full_path[256];
    winsize_calculate();
    wclear(contents);

    int row_pos = win_row / 2 - 2;
    int col_pos = win_col / 2 - 1;
    mvwaddstr(contents, row_pos++, win_col / 2 - 18, "write down file name you want to make");
    wrefresh(contents);
    echo();
    mvwscanw(contents, row_pos, col_pos, "%s", file_name);

    if (getcwd(full_path, 256) == NULL) {
        perror("getcwd");
        noecho();
        return;
    }
    strcat(full_path, "/");
    strcat(full_path, file_name);

    fd = creat(file_name, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd == -1) {
        perror("file creat failed");
	    noecho();
        return;
    }

    if (close(fd) == -1) {
    	perror("file close failed");
        noecho();
        return;
    }
    noecho();
    file_open(file_name, full_path);
    code_tab_transition();
}

void make_makefile() {
    FILE *fp;
    int temp;
    char file_name[256];
    char makefile_header[256];
    char *makefile_content;

    winsize_calculate();
    wclear(contents);

    int row_pos = win_row / 2 - 2; int col_pos = win_col / 2 - 3;
    mvwaddstr(contents, row_pos++, win_col / 2 - 16, "Do you want to make a makefile?");
    mvwaddch(contents, row_pos, col_pos++, '[');
    wattron(contents, A_UNDERLINE);
    mvwaddch(contents, row_pos, col_pos++, 'Y');
    wattroff(contents, A_UNDERLINE);
    mvwaddstr(contents, row_pos, col_pos, "es]");
    wrefresh(contents);
    temp = getch();

    const char *variables =
        "CC = gcc\n"
        "CFLAGS = -lcurses\n"
        "SRCS = $(wildcard *.c)\n"
        "OBJS = $(SRCS:.c=.o)\n"
        "TARGET = ";

    const char *rules =
        "$(TARGET) : $(OBJS)\n"
	    "\t$(CC) -o $@ $(OBJS) $(CFLAGS)\n\n"
        "%.o: %.c %.h\n"
        "\t$(CC) -c $< $(CFLAGS)\n\n"
    	"%.o: %.c\n"
	    "\t$(CC) -c $< $(CFLAGS)\n\n"
	    ".PHONY: clean\n";
        "clean:\n"
        "\trm -f $(TARGET) $(OBJS)\n\n";


    if (temp == 'y' || temp == 'Y') {
        wclear(contents);
        row_pos = win_row / 2 - 2;
        col_pos = win_col / 2 - 1;
        mvwaddstr(contents, row_pos++, win_col / 2 - 15, "write down name of executable");
        echo();
        mvwscanw(contents, row_pos, col_pos, "%s", file_name);

	    strcpy(makefile_header, variables);
    	strcat(makefile_header, file_name);
    	strcat(makefile_header, "\n\n");

        size_t content_size = strlen(makefile_header) + strlen(rules) + 1;
        makefile_content = malloc(content_size);
        if (makefile_content == NULL) {
                perror("Failed to allocate memory");
                return;
        }

        // string merge
        strcpy(makefile_content, makefile_header);
        strcat(makefile_content, rules);

        // Makefile creat
        fp = fopen("Makefile", "w");
        if (fp == NULL) {
            perror("failed to make Makefile");
            free(makefile_content);
            return;
        }

        fprintf(fp, "%s", makefile_content);
        fclose(fp);
        free(makefile_content);
    }
    // noecho();
    workspace_contents_print();
}

WorkSpaceFile *get_cur_workspace() {
    WorkSpaceFile *cur;
    cur = contents_head;
    for (int i = 0; i < workspace_contents_row+workspace_file_focus; i++)
    {
        cur = cur->next;
    }

    return cur;
}