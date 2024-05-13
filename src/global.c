#include "global.h"

void flag_init() {
    flag = 0;
}

int get_winsize_changed() {
    return flag & WINSIZE_CHANGED;
}

void set_winsize_changed(int val) {
    if(val)
        flag = flag | WINSIZE_CHANGED;
    else
        flag = flag & ~WINSIZE_CHANGED;
}

int get_wait_any_input() {
    return flag & WAIT_ANY_INPUT;
}

void set_wait_any_input(int val) {
    if(val)
        flag = flag | WAIT_ANY_INPUT;
    else
        flag = flag & ~WAIT_ANY_INPUT;
}