#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "lcd_menu.h"
#include "lcd_hd44780.h"

void init_interrput_vars(void);

void handle_center_btn(void);

void handle_left_btn(void);



#endif //INTERRUPTS_H
