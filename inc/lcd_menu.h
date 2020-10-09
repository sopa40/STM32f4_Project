#ifndef LCD_MENU_H
#define LCD_MENU_H

#include "lcd_hd44780.h"
#include "tick.h"
#include "password.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

enum MENU_STATUS {
	MENU_INIT,
	ENTER_PASS,
	ENTER_MASTER_PASS,
	WRONG_PASS,
	SET_PASS,
	ACCESS_GRANTED,
	ACCESS_DENIED,
	MASTER_ACCESS_GRANTED,
	MASTER_ACCESS_DENIED
};

struct Menu {
	enum MENU_STATUS status;
	uint8_t pos;
	bool row;
	uint8_t pwd_pos;
};

struct Menu *get_lcd_menu (void);

void init_menu_vars(void);

void init_lcd_with_settings(void);

void print_error(const char *str);

void draw_master_pass_input(void);

void draw_pass_input(void);

void move_cursor_left(void);

void move_cursor_right(void);

void hide_sym(void);

void show_sym(bool is_master);

#endif //LCD_MENU_H
