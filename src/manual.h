#ifndef __MANUAL_TAB
#define __MANUAL_TAB

#include "global.h"

#define MAN_PAGE_CNT 4
#define MAN_PAGE_NAME_LEN 6

extern enum ManualPage manual_page_focus;
extern char manual_page_names[MAN_PAGE_CNT][MAN_PAGE_NAME_LEN];

void manual_tab_transition();
void manual_page_print();

#endif