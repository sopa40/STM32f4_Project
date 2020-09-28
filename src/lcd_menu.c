#include "lcd_menu.h"
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


void draw_pass_input(void)
{
    sk_lcd_cmd_clear(lcd);
    lcd_putstring(lcd, "     ******      ");
    lcd_menu.
    //sk_lcd_cmd_setaddr(&lcd, 0x40, false);
}
