#ifndef LCD_MENU_H
#define LCD_MENU_H

#include "lcd_hd44780.h"
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





#endif //LCD_MENU_H
