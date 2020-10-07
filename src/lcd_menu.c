#include "lcd_menu.h"
#include "lcd_hd44780.h"
#include <stdbool.h>

#include <stdio.h>


//TODO: add pass length setting in master menu
#define MAX_DISPLAY_LENGTH 16
#define PASS_LENGTH 6
#define PASS_START_POS (MAX_DISPLAY_LENGTH - PASS_LENGTH) / 2

static struct sk_lcd *lcd = NULL;
//static struct Password password = NULL;

void init_lcd_with_settings(void)
{
    sk_pin_group_set(sk_io_lcd_data, 0x00);
	sk_lcd_init(lcd);
	sk_lcd_cmd_onoffctl(lcd, true, false, false);
	sk_lcd_set_backlight(lcd, 200);
	sk_lcd_cmd_setaddr(lcd, 0x00, false);
	lcd_putstring(lcd, "   Press OK");
	sk_lcd_cmd_setaddr(lcd, 0x40, false);
	lcd_putstring(lcd, " To enter pass");
}

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

static void return_cursor_back(void)
{
    if (!lcd_menu.row)
        sk_lcd_cmd_setaddr(lcd, 0x00, false);
    else
        sk_lcd_cmd_setaddr(lcd, 0x40, false);

    for(int i = 0; i < lcd_menu.position; i++)
        sk_lcd_cmd_shift(lcd, false, true);

    sk_lcd_cmd_onoffctl(lcd, true, true, true);
}

static void clear_bottom_row(void)
{
    sk_lcd_cmd_setaddr(lcd, 0x40, false);
    //TODO: refactor. Now clearing is simple brute force
    lcd_putstring(lcd, "                ");
    return_cursor_back();
}

void print_error(const char *str)
{
    clear_bottom_row();
    sk_lcd_cmd_setaddr(lcd, 0x40, false);
    lcd_putstring(lcd, str);
    return_cursor_back();
}

void draw_pass_input(void)
{
    sk_lcd_cmd_clear(lcd);
    //TODO: refactor. Now drawing is simple brute force
    lcd_putstring(lcd, "     ******      ");
    lcd_menu.position = 5;
    lcd_menu.row = 0;
    return_cursor_back();
}

void move_cursor_left(void)
{
    if(PASS_START_POS != lcd_menu.position) {
        lcd_menu.position--;
        sk_lcd_cmd_shift(lcd, false, false);
    } else {
        print_error("can't move left!");
        sk_tick_delay_ms(1500);
        clear_bottom_row();
    }
}

void move_cursor_right(void)
{
    if((PASS_START_POS + PASS_LENGTH) - 1 != lcd_menu.position) {
        lcd_menu.position += 1;
        sk_lcd_cmd_shift(lcd, false, true);
    } else {
        print_error("can't move right!");
        sk_tick_delay_ms(1500);
        clear_bottom_row();
    }
}

void show_symbol(void)
{

}
