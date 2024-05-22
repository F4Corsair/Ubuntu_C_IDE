#include <stdio.h>
#include <unistd.h>

#include "input.h"
#include "global.h"
#include "uibase.h"
#include "code.h"
#include "file.h"
#include "build.h"
#include "terminal.h"
#include "manual.h"
#include "quit.h"
#include "winsize.h"
#include "fileSave.h"


int input_control(int input_char) {
    int idx;
    // input ignore
    if (input_char == 0x19a || winsize_flag == 1) {
        // ignore KEY_RESIZE
        return 0;
    }
    // check tab transition
    switch (input_char)
    {
    case CTRL('c'):
        code_tab_transition();
        return 0;
    case CTRL('f'):
        file_tab_transition();
        return 0;
    case CTRL('b'):
        build_tab_transition();
        return 0;
    case CTRL('t'):
        terminal_tab_transition();
        return 0;
    case 0xa: // to recognize CTRL_M
        manual_tab_transition();
        return 0;
    case CTRL('q'):
        quit_tab_transition();
        return 0;
    default:
        break;
    }

    if(menu_tab_focus == CODE_TAB) { // code tab requires numerous input - so check it first
        FileStatus *focus = opened_file_info->focus;
        if(unsaved_caution_flag != 0) { // caution if user tried to close unsaved file
            idx = unsaved_caution_flag - 1;
            if(input_char == 's' || input_char == 'S') {
                // save & del & refresh
                // save_file(idx);
                // del_opened_file_tab(idx);
            } else if(input_char == 'c' || input_char == 'C') {
                // don't save & del
                // find file
                FileStatus *ptr = opened_file_info->head;
                while (idx > 0)
                {
                    if(ptr == NULL) {
                        perror("close unsaved file : out of index");
                        unsaved_caution_flag = 0;
                        return 0;
                    }
                    ptr = ptr->next;
                    idx--;
                }
                ptr->modified = 0; // force status change as unmodified(saved)
                del_opened_file_tab(idx);
            } else {
                opened_file_tab_print();
                code_contents_print();
            }
            unsaved_caution_flag = 0;
            return 0;
        } else if (focus->fd == -1) { // file not opened
            switch (input_char)
            {
            case CTRL('w'):
                idx = opened_file_focus_idx_find();
                if(idx != -1) {
                    del_opened_file_tab(idx);
                }
                break;
            case 0x226:
            case 0x227: // ctrl + PGDN : move opened file tab focus down
                opened_file_focus_next();
                opened_file_tab_print();
                // todo : tmp save
                code_contents_print();
                break;
            case 0x22b:
            case 0x22c: // ctrl + PGUP : move opened file tab foucs up
                opened_file_focus_prev();
                opened_file_tab_print();
                // todo : tmp save
                code_contents_print();
                break;
            }
            return 0;
        }
        switch (input_char)
        {
        case CTRL('w'):
            idx = opened_file_focus_idx_find();
            if(idx != -1) {
                del_opened_file_tab(idx);
            }
            break;
        case 0x226:
        case 0x227: // ctrl + PGDN : move opened file tab focus down
            opened_file_focus_next();
            opened_file_tab_print();
            code_contents_print();
            break;
        case 0x22b:
        case 0x22c: // ctrl + PGUP : move opened file tab foucs up
            opened_file_focus_prev();
            opened_file_tab_print();
            code_contents_print();
            break;
        case 0x102: // down arrow
            if(code_next_row_exists() != -1) {
                focus->row++;
                if(focus->row - focus->start_row >= win_row - 3) {
                    focus->start_row++;
                    CodeLine *ptr = focus->buf->cur;
                    if(ptr->next != NULL) {
                        focus->buf->cur = ptr->next;
                    }
                }
                // check col
                int col_max_len = get_cur_code_line_len() - 1;
                if(col_max_len < focus->col) {
                    if(col_max_len <= 0)
                        focus->col = 0;
                    else
                        focus->col = col_max_len;
                }
                code_contents_print();
            }
            break;
        case 0x103: // up arrow
            if(focus->row > 0) {
                focus->row--;
                if(focus->start_row > focus->row) {
                    focus->start_row--;
                    CodeLine *ptr = focus->buf->cur;
                    if(ptr->prev != NULL) {
                        focus->buf->cur = ptr->prev;
                    }
                }
                // check col
                int col_max_len = get_cur_code_line_len() - 1;
                if(col_max_len < focus->col) {
                    if(col_max_len <= 0)
                        focus->col = 0;
                    else
                        focus->col = col_max_len;
                }
                code_contents_print();   
            }            
            break;
        case 0x104: // left arrow
            if(focus->col > 0) {
                focus->col--;
                if(focus->start_col > focus->col) {
                    focus->start_col--;
                }
                code_contents_print();
            }
            break;
        case 0x105: // right arrow
            if(code_next_col_exists() != -1) {
                focus->col++;
                if(focus->start_col - focus->col >= win_col) {
                    focus->start_col++;
                }
                code_contents_print();
            }
            break;
        case 0x152: // PGDN
            for(int i = 0; i < win_row - 3; i++) {
                if(code_next_row_exists() != -1) {
                    focus->row++;
                    if(focus->row - focus->start_row >= win_row - 3) {
                        focus->start_row++;
                        CodeLine *ptr = focus->buf->cur;
                        if(ptr->next != NULL) {
                            focus->buf->cur = ptr->next;
                        }
                    }
                    code_contents_print();
                }
            }
            break;
        case 0x153: // PGUP
            for(int i = 0; i < win_row - 3; i++) {
                if(focus->row > 0) {
                    focus->row--;
                    if(focus->start_row > focus->row) {
                        focus->start_row--;
                        CodeLine *ptr = focus->buf->cur;
                        if(ptr->prev != NULL) {
                            focus->buf->cur = ptr->prev;
                        }
                    }
                    code_contents_print();   
                }  
            }
            break;
        case 0x13: // ctrl + s
            file_save_focus();
            opened_file_tab_print();
            code_contents_print();
            break;
        default:
            // input char handling
            break;
        }
    } else {
        switch (menu_tab_focus)
        {
        case FILE_TAB:
            break;
        case BUILD_TAB:
            break;
        case TERMINAL_TAB:
            break;
        case MANUAL_TAB:
            if(input_char == 0x104) { // KEY_LEFT
                manual_page_focus = (manual_page_focus - 1) % MAN_PAGE_CNT;
                manual_page_print();
            } else if(input_char == 0x105) { // KEY_RIGHT
                manual_page_focus = (manual_page_focus + 1) % MAN_PAGE_CNT;
                manual_page_print();
            }
            break;
        case QUIT_TAB:
            if(input_char == 'y' || input_char == 'Y') {
                return -1;
            } else {
                tab_restore();
            }
            break;
        case WINSIZE_TAB:
            if(winsize_flag == 2) {
                // ready to reset stdscr
                winsize_calculate();
                window_reset();
                tab_restore();
            }
            break;
        default:
            perror("menu_tab_focus out of index");
        }

    }

    return 0;
}