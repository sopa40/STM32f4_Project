#include "lcd_menu.h"
#include "lcd_hd44780.h"
#include <stdbool.h>

static struct sk_lcd *lcd = NULL;

void init_menu_vars(void)
{
    lcd = get_lcd();
}

static struct Menu lcd_menu = {
	.status = MENU_INIT,
	.position = 0,
	.row = 0,
};

struct Menu *get_lcd_menu(void)
{
    return &lcd_menu;
}

void print_error(const char *str)
{
    sk_lcd_cmd_setaddr(lcd, 0x40, false);
    lcd_putstring(lcd, "                "); //brute force clearing row. To be refactored
    sk_lcd_cmd_setaddr(lcd, 0x40, false);
    lcd_putstring(lcd, str);
}

void draw_pass_input(void)
{
    sk_lcd_cmd_clear(lcd);
    lcd_putstring(lcd, "     ******      ");
}

void move_cursor_left(void)
{
    if(!lcd_menu.position) {
        print_error("can't move left!");
    } else {
        lcd_menu.position--;
    }
}
