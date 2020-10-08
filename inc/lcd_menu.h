#ifndef LCD_MENU_H
#define LCD_MENU_H

#include "lcd_hd44780.h"
#include "tick.h"
#include "password.h"
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
	uint8_t pass_symbol_pos;
};

struct Menu *get_lcd_menu (void);

void init_menu_vars(void);

void init_lcd_with_settings(void);

void print_error(const char *str);

void draw_pass_input(void);

void move_cursor_left(void);

void move_cursor_right(void);

void hide_symbol(void);

void show_symbol(void);

#endif //LCD_MENU_H
