#include "lcd_menu.h"
#include "lcd_hd44780.h"
#include "password.h"
#include <stdbool.h>

#include <stdio.h>


//TODO: add pwd length setting in master menu
#define MAX_DISPLAY_LENGTH 16

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
	lcd_putstring(lcd, " To enter pwd");
}

void init_menu_vars(void)
{
    lcd = get_lcd();
}

static struct Menu lcd_menu = {
	.status = MENU_INIT,
	.pos = 0,
	.row = 0,
    .pwd_pos = 10   //invalid value
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

    for(int i = 0; i < lcd_menu.pos; i++)
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

void draw_pwd_input(bool is_master)
{
    sk_lcd_cmd_clear(lcd);
    if (is_master) {
        //TODO: refactor. Now drawing is simple brute force
        lcd_putstring(lcd, "      ****       ");
        lcd_menu.pos = 6;
        show_sym(true);
    } else {
        lcd_putstring(lcd, "     ******      ");
        lcd_menu.pos = 5;
        show_sym(false);
    }
    lcd_menu.row = 0;
    lcd_menu.pwd_pos = 0;
    return_cursor_back();
}

void print_options(void)
{
    sk_lcd_cmd_clear(lcd);
    lcd_putstring(lcd, " Change pwd");
    sk_lcd_cmd_setaddr(lcd, 0x40, false);
    lcd_putstring(lcd, " Exit");
}

void move_cursor_left(void)
{
    if(0 != lcd_menu.pwd_pos) {
        lcd_menu.pos--;
        lcd_menu.pwd_pos--;
        sk_lcd_cmd_shift(lcd, false, false);
    } else {
        print_error("can't move left!");
        sk_tick_delay_ms(1500);
        clear_bottom_row();
    }
}

void move_cursor_right(void)
{
    if(get_pwd_len() - 1 != lcd_menu.pwd_pos) {
        lcd_menu.pos++;
        lcd_menu.pwd_pos++;
        sk_lcd_cmd_shift(lcd, false, true);
    } else {
        print_error("can't move right!");
        sk_tick_delay_ms(1500);
        clear_bottom_row();
    }
}

void hide_sym(void)
{
    sk_lcd_putchar(lcd, '*');
    sk_lcd_cmd_shift(lcd, false, false);
}

void show_sym(bool is_master)
{
    char symbol = get_pwd_sym(lcd_menu.pwd_pos, is_master);
    if(255 != symbol) {
        sk_lcd_putchar(lcd, symbol);
        sk_lcd_cmd_shift(lcd, false, false);
    } else
        print_error("False symbol pos\n");
}
