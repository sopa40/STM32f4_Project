#ifndef LCD_MENU_H
#define LCD_MENU_H

#include "lcd_hd44780.h"
#include "tick.h"
#include "password.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/** enum for menu navigation */
enum MENU_STATUS {
	MENU_INIT,
	ENTER_PWD,
	ENTER_MASTER_PWD,
	WRONG_PWD,
	PWD_NOT_SET,
	ACCESS_GRANTED,
	MASTER_ACCESS_GRANTED,
	CHANGE_PWD,
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


/** initializes variables (lcd display) */
void init_menu_vars(void);

/** initalizes lcd display and sets some setting */
void init_lcd_with_settings(void);

/** prints welcome message to the lcd display */
void print_welcome_msg(void);

/** prints message when pwd is not set */
void print_no_pwd_msg(void);

/** prints wait time on the bottom row of lcd display */
void print_wait_time(uint32_t time);

/** clears top row of lcd display and prints string */
void print_info(const char *str);

/** clear botoom row of lcd display and prints string */
void print_error(const char *str);

/** draws password symbols and shows first */
void draw_pwd_input(bool is_master);

/** moves cursor to another row (used in master menu for navigation) */
void move_row_curs(void);

/** prints first option list */
void print_options(void);

/** prints second option list */
void print_next_options(void);

/** moves cursor one symbol to the left */
void move_cursor_left(void);

/** moves cursor one symbol to the right */
void move_cursor_right(bool is_master);

/** hides symbol when moving to another password symbol */
void hide_sym(void);

/** shows current symbol on cursor position */
void show_sym(bool is_master);

/** imitates lock opening */
void open_lock(void);

/** imitates lock closing */
void close_lock(void);

#endif //LCD_MENU_H
