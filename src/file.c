#include <curses.h>
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>

#include "global.h"
#include "code.h"
#include "winsize.h"
#include "openedFileTab.h"
#include "file.h"
#include "uibase.h"

#define KEY_NAME_WIDTH 14
#define FILE_TAB_WIDTH 15

int max_file_tab; // 현재 창 크기에 따라 결정됨
int file_tab_cnt;
int file_tab_focus;
int contents_row;
int contents_col;
int workspace_contents_row;
int workspace_contents_col;
int workspace_file_focus=0;     //contents win 에서 첫번째 나오는 파일이름이 연결리스트에서 몇번째 파일인지
int workspace_file_finish=0;

FileStatus* head;

WINDOW* workspace_tab;


void file_tab_transition() {
    if(menu_tab_focus == FILE_TAB)
        return;

    erase();
    refresh();

    menu_tab_focus = FILE_TAB;
    menu_tab_update();
    wrefresh(menu_tab);

    // todo : show FILE_TAB
    opened_workspace_tab_print();
    workspace_contents_print();

}

void file_open_update(int *input_char) {
	winsize_calculate();
	wclear(contents);

	int row_pos = win_row / 2 - 2;
   	mvwaddstr(contents, row_pos++, win_col / 2 - 10, "Code file tab is full!");
	mvwaddstr(contents, row_pos++, win_col / 2 - 15, "Do you really want to open the file?");
    int col_pos = win_col / 2 - 1;
    	mvwaddch(contents, row_pos, col_pos++, '[');
    	wattron(contents, A_UNDERLINE);
    	mvwaddch(contents, row_pos, col_pos++, 'Y');
    	wattroff(contents, A_UNDERLINE);
    	mvwaddstr(contents, row_pos, col_pos, "es]");
	
	wrefresh(contents);

	*(input_char) = getch();
}

/* int find_most_previous_file() {
	FileStatus *cur = opened_file_info->head;
    FileStatus *most_previous_file = NULL;

	// head가 가리키는 node의 prev node가 가장 나중에 만들어진 node
	int index = 0;
	do {
		most_previous_file = cur;
		cur = cur->next;
		index++;
	} while (cur->next != opened_file_info->head);

	return index;
} */

void contents_window_restore() {
	enum MenuTab focus;
	wclear(contents);

	if (menu_tab_focus == CODE_TAB) {
		opened_file_tab_print();
		code_contents_print();
	}
	else if (menu_tab_focus == FILE_TAB) {
		opened_workspace_tab_print();
		workspace_contents_print();
	}
}

void file_open(char *file_name, int new_file_input) {
	// int new_file_input;
	char path[256];

	// full_path
	if (getcwd(path, 256) == NULL) {
		perror("getcwd");
		exit(1);
	}
	strcat(path, "/");
	strcat(path, file_name);
	
	// file existence check - exception handling
	if (access(path, F_OK) != 0) {
		perror("file does not exist");
		exit(1);
	}

	// to do : input control
	int flag = new_opened_file_tab(file_name, path);
	int input_char;
        if (flag == -1) {
                file_open_update(&input_char);
		
		// new_file_input = getch();
		if (new_file_input == 'y' || new_file_input == 'Y') {
			// todo : index 지정 정확하게 하기
			del_opened_file_tab(8);
			new_opened_file_tab(file_name, path);
			opened_file_tab_print();
			code_contents_print();
		}
		else // todo : make new contents restore code
			contents_window_restore();
	}
}

void print_path(const char *path) {
    int width = COLS - 2; // 테두리를 위한 공간 확보
    wattron(contents, A_UNDERLINE);
    mvwprintw(contents, 0, 0, "Path:");
    mvwprintw(contents, 0, 5, " %-*s", width, path); // "Path: " 공간 확보
    wattroff(contents, A_UNDERLINE);
    wrefresh(contents);
}

void initialize_colors() {
    start_color(); // 색상 사용을 위해 초기화
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    attron(COLOR_PAIR(1));
    refresh();
}

// 영준이 todo
void opened_workspace_tab_print() {
	initialize_colors();

    max_file_tab = COLS/FILE_TAB_WIDTH-1;
    file_tab_cnt = 0;
    file_tab_focus = -1;

    new_file_tab();
    //mvwprintw(opened_file_tab, 0, KEY_NAME_WIDTH - 1, "/");
    wrefresh(opened_file_tab);


    attroff(COLOR_PAIR(1));
}
void new_file_tab(){
    int start_pos = 0;
    char file_name[FILE_TAB_WIDTH];
    if(file_tab_cnt >= max_file_tab) {
        // todo : delete tab by policy
    } else
        file_tab_cnt++;
    for(file_tab_focus=0;file_tab_focus<file_tab_cnt;file_tab_focus++){
        start_pos = KEY_NAME_WIDTH + FILE_TAB_WIDTH * (file_tab_focus - 1); 
        
    // print tab
        snprintf(file_name, sizeof(file_name), "WorkSpace%d", file_tab_focus+1);  
        mvwprintw(opened_file_tab, 0, start_pos, "\\");
        if(file_tab_focus==file_tab_cnt-1)
             wattron(opened_file_tab, A_STANDOUT|A_UNDERLINE);
        else
            wattron(opened_file_tab, A_UNDERLINE);
        mvwprintw(opened_file_tab, 0, start_pos+1, "%-*s", FILE_TAB_WIDTH - 2, file_name);
        if(file_tab_focus==file_tab_cnt-1)
             wattroff(opened_file_tab, A_STANDOUT|A_UNDERLINE);
        else
            wattroff(opened_file_tab, A_UNDERLINE);
        mvwprintw(opened_file_tab, 0, start_pos+FILE_TAB_WIDTH-1, "/");

        wrefresh(opened_file_tab);
    }
    // calculate start_pos
    
}

void ls(char *path) {
    contents_col = 1;
    contents_row = 1;

    head=NULL;
    start_color();
    init_pair(2, COLOR_RED, COLOR_BLACK);
    init_pair(3, COLOR_BLUE, COLOR_BLACK);
    lsR(path);
    /*
    FileStatus *cur;
        cur=head;
        for(;cur;cur=cur->next){
            mvwprintw(contents,contents_row++,contents_col,"%s %s",cur->file_name,cur->full_path);
        }
    */
    wrefresh(contents);
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

void lsR(char *path) {
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
        addToList(direntp->d_name,full_path);
        if (S_ISDIR(info.st_mode)) {
            wattron(contents, COLOR_PAIR(2));
            mvwprintw(contents, contents_row,contents_col, "v ");
            wattroff(contents, COLOR_PAIR(2)); 
            mvwprintw(contents, contents_row++, contents_col+2, "%s", direntp->d_name);
            
            contents_col += 2;
            lsR(full_path);
            contents_col = 1;
        } else {
            if (contents_col == 1)
                mvwprintw(contents, contents_row++, contents_col, "%s", direntp->d_name);
            else{
                wattron(contents, COLOR_PAIR(3));
                mvwprintw(contents, contents_row, contents_col, "> ");
                wattroff(contents, COLOR_PAIR(3)); 
                mvwprintw(contents, contents_row++, contents_col+2, "%s", direntp->d_name);
            }
        }
        

        /**/
        
    }

    closedir(dir_ptr);
}

void workspace_contents_print() {
	char path[256];
    workspace_contents_row=1;
    workspace_contents_col=1; // 파일 선택을 위한 커서
    if (getcwd(path, sizeof(path)) != NULL) {
        print_path(path);
        ls(path);
        wmove(contents, workspace_contents_row, 0); // 줄의 시작으로 이동
        wclrtoeol(contents);
        mvwprintw(contents,workspace_contents_row,workspace_contents_col,head->file_name);
        mvwchgat(contents, workspace_contents_row, workspace_contents_col, strlen(head->file_name), A_BLINK, 0, NULL);
    }
    wrefresh(contents);
}

void workspace_key_down(){
    if (workspace_contents_row == win_row - 2 && workspace_file_finish != -1) {
        // 화면의 마지막 줄에 도달한 경우
        workspace_file_focus++;
        FileStatus* cur = head;

        for (int i = 0; i < workspace_file_focus; i++) {
            if (cur == NULL) {
                workspace_file_finish = -1;
                break;
            }
            cur = cur->next;
        }

        if (cur != NULL) {
            for (int row = 1; row <= win_row - 2; row++) {
                wmove(contents, row, 0); // 줄의 시작으로 이동
                wclrtoeol(contents);
                if (cur != NULL) {
                    mvwprintw(contents, row, 1, cur->file_name);
                    cur = cur->next;
                } else {
                    workspace_file_finish = -1;
                    break;
                }
            }
            mvwchgat(contents, win_row - 2, 1, strlen(cur->file_name), A_BLINK, 0, NULL);
            wrefresh(contents);
        }
    } else if (workspace_contents_row <=win_row-3) {
        // 화면 내에서 이동
        wchgat(contents, -1, A_NORMAL, 0, NULL);
        if (workspace_contents_row < win_row - 2) {
            workspace_contents_row++;
        }
        FileStatus* cur = head;
        for (int i = 0; i < workspace_contents_row - 1 + workspace_file_focus; i++) {
            cur = cur->next;
        }
        if (cur != NULL) {
            wmove(contents, workspace_contents_row, 0); // 줄의 시작으로 이동
            wclrtoeol(contents);
            mvwprintw(contents, workspace_contents_row, workspace_contents_col, cur->file_name);
            mvwchgat(contents, workspace_contents_row, workspace_contents_col, strlen(cur->file_name), A_BLINK, 0, NULL);
            wrefresh(contents);
        }
    }
}

void workspace_key_up(){
    if (workspace_contents_row == 1 && workspace_file_focus > 0) {
        // 화면의 첫 줄에 도달하고, 이전 파일이 존재하는 경우
        if(workspace_file_finish==-1)
            workspace_file_finish=0;
        workspace_file_focus--;
        FileStatus* cur = head;
        for (int i = 0; i < workspace_file_focus; i++) {
            cur = cur->next;
        }
        for (int row = 1; row <= win_row - 3; row++) {
            wmove(contents, row, 0); // 줄의 시작으로 이동
            wclrtoeol(contents);
            if (cur != NULL) {
                mvwprintw(contents, row, 1, cur->file_name);
                cur = cur->next;
            }
        }
        mvwchgat(contents, 1, 1, strlen(cur->file_name), A_BLINK, 0, NULL);
        wrefresh(contents);
    } else if (workspace_contents_row > 1) {
        // 화면 내에서 이동
        wchgat(contents, -1, A_NORMAL, 0, NULL);
        workspace_contents_row--;
        FileStatus* cur = head;
        for (int i = 0; i < workspace_contents_row - 1 + workspace_file_focus; i++) {
            cur = cur->next;
        }
        if (cur != NULL) {
            wmove(contents, workspace_contents_row, 0); // 줄의 시작으로 이동
            wclrtoeol(contents);
            mvwprintw(contents, workspace_contents_row, workspace_contents_col, cur->file_name);
            mvwchgat(contents, workspace_contents_row, workspace_contents_col, strlen(cur->file_name), A_BLINK, 0, NULL);
            wrefresh(contents);
        }
    }
}



// 파일 이름이 .h로 끝나는지 확인하는 함수
bool has_extension(const char *filename, const char *extension) {
    size_t len = strlen(filename);
    size_t ext_len = strlen(extension);
    if (len < ext_len) return false;
    return strcmp(filename + len - ext_len, extension) == 0;
}


// 파일 이름이 .c 또는 .h로 끝나는지 확인하는 함수