#ifndef LCD_MENU_H
#define LCD_MENU_H

#include "lcd_hd44780.h"
#include "tick.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

enum MENU_STATUS {
	MENU_INIT = 0,
	ENTER_PASS = 1,
	WRONG_PASS = 2,
	SET_PASS = 3,
	ACCESS_GRANTED = 4,
	ACCESS_DENIED = 5
};

struct Menu {
	enum MENU_STATUS status;
	uint8_t position;
	bool row;
};

struct Menu *get_lcd_menu (void);

void init_menu_vars(void);

void draw_pass_input(void);

#endif //LCD_MENU_H
