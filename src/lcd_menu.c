#include "lcd_menu.h"
#include "lcd_hd44780.h"
#include "password.h"
#include <stdbool.h>

#include <stdio.h>


//TODO: add pass length setting in master menu
#define MAX_DISPLAY_LENGTH 16
#define PASS_LEN 6

const uint8_t pass_len = 6;

static struct sk_lcd *lcd = NULL;

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
    .pass_symbol_pos = 10   //invalid value
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
    lcd_menu.pass_symbol_pos = 0;
    return_cursor_back();
    show_symbol();
}

void move_cursor_left(void)
{
    if(0 != lcd_menu.pass_symbol_pos) {
        lcd_menu.position--;
        lcd_menu.pass_symbol_pos--;
        sk_lcd_cmd_shift(lcd, false, false);
    } else {
        print_error("can't move left!");
        sk_tick_delay_ms(1500);
        clear_bottom_row();
    }
}

void move_cursor_right(void)
{
    if(get_pass_len() - 1 != lcd_menu.pass_symbol_pos) {
        lcd_menu.position++;
        lcd_menu.pass_symbol_pos++;
        sk_lcd_cmd_shift(lcd, false, true);
    } else {
        print_error("can't move right!");
        sk_tick_delay_ms(1500);
        clear_bottom_row();
    }
}

void hide_symbol(void)
{
    sk_lcd_putchar(lcd, '*');
    sk_lcd_cmd_shift(lcd, false, false);
}

void show_symbol(void)
{
    char symbol = get_pass_symbol(lcd_menu.pass_symbol_pos);
    if(255 != symbol) {
        sk_lcd_putchar(lcd, symbol);
        sk_lcd_cmd_shift(lcd, false, false);
    } else
        print_error("False symbol pos\n");
}
