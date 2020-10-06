#include "interrupts.h"
#include "lcd_menu.h"

#include <stdio.h>

static struct sk_lcd *lcd = NULL;
static struct Menu *lcd_menu = NULL;

void init_interrput_vars(void)
{
    lcd = get_lcd();
	lcd_menu = get_lcd_menu();
}

void handle_center_btn(void)
{
    switch(lcd_menu->status) {
        case MENU_INIT:
            draw_pass_input();
            lcd_menu->status = ENTER_PASS;
            break;
        case ENTER_PASS:
            sk_lcd_cmd_clear(lcd);
            break;
        default:
            sk_lcd_cmd_clear(lcd);
            lcd_putstring(lcd, "some error");
            break;
    }
	sk_lcd_cmd_onoffctl(lcd, true, true, true);
}
