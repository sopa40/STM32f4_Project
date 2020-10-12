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
	ENTER_PWD,
	ENTER_MASTER_PWD,
	WRONG_PWD,
	PWD_NOT_SET,
	ACCESS_GRANTED,
	MASTER_ACCESS_GRANTED,
	CHANGE_PWD,
	PWD_CHANGED,
	OPTIONS1,
	OPTIONS2,
	FATAL_ERROR
};

struct Menu {
	enum MENU_STATUS status;
	uint8_t pos;
	bool row;
	uint8_t pwd_pos;
	bool is_in_master;
};

struct Menu *get_lcd_menu (void);

void init_menu_vars(void);

void init_lcd_with_settings(void);

void print_welcome_msg(void);

void print_no_pwd_msg(void);

void print_wait_time(uint32_t time);

void print_info(const char *str);

void print_error(const char *str);

void draw_pwd_input(bool is_master);

void show_row_cursor(bool row);

void move_row_curs(void);

void print_options(void);

void print_next_options(void);

void move_cursor_left(void);

void move_cursor_right(bool is_master);

void hide_sym(void);

void show_sym(bool is_master);

void open_lock(void);

void close_lock(void);

#endif //LCD_MENU_H
