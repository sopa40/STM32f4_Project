#include "lcd_menu.h"
#include "lcd_hd44780.h"
#include "password.h"
#include <stdbool.h>

#include <stdio.h>

const char ii_code [] = {
    0x00,
    0x0A,
    0x00,
    0x0C,
    0x04,
    0x04,
    0x04,
    0x00,
};

const char je_code [] = {
	0x00,
	0x00,
	0x06,
	0x09,
	0x0C,
	0x09,
	0x06,
	0x00
};


//hot fix: ukrainian letter 'ґ' will be printed as an arrow
const char arr_code [] = {
    0x00,
    0x04,
    0x08,
    0x1F,
    0x08,
    0x04,
    0x00,
    0x00
};

//TODO: add pwd length setting in master menu
#define MAX_DISPLAY_LENGTH 16

static struct sk_lcd *lcd = NULL;


static struct Menu lcd_menu = {
	.status = MENU_INIT,
	.pos = 0,
	.row = 0,
    .pwd_pos = 10,   //invalid value
    .is_in_master = false
};

struct Menu *get_lcd_menu(void)
{
    return &lcd_menu;
}

void init_menu_vars(void)
{
    lcd = get_lcd();
}

/** adds custom letter to CGRAM memory of display
 *  @lcd_disp: LCD display
 *  @addr: CGRAM address, where symbol should be written
 *  @letter_code: byte array of symbol code
*/
static void _add_letter(struct sk_lcd *lcd_disp, uint8_t addr, const char letter_code[])
{
	sk_lcd_cmd_setaddr(lcd_disp, addr, true);
	for (int i = 0; i < 8; i++) {
		sk_lcd_write_byte(lcd_disp, letter_code[i]);
	}
}

/** adds couple of letters at a time */
static void add_letters(void) {
    _add_letter(lcd, 0x00, ii_code);
    _add_letter(lcd, 0x08, je_code);
    _add_letter(lcd, 0x10, arr_code);
}

void init_lcd_with_settings(void)
{
    sk_pin_group_set(sk_io_lcd_data, 0x00);
	sk_lcd_init(lcd);
	sk_lcd_cmd_onoffctl(lcd, true, false, false);
	sk_lcd_set_backlight(lcd, 200);
    add_letters();
    print_welcome_msg();
}

/** returns cursor (sets lcd DGRAM address) to the position specified in lcd_menu struct */
static void return_cursor_back(void)
{
    if (!lcd_menu.row)
        sk_lcd_cmd_setaddr(lcd, 0x00, false);
    else
        sk_lcd_cmd_setaddr(lcd, 0x40, false);

    for(int i = 0; i < lcd_menu.pos; i++)
        sk_lcd_cmd_shift(lcd, false, true);
}

/** clears top row content of lcd display */
static void clear_top_row(void)
{
    sk_lcd_cmd_setaddr(lcd, 0x00, false);
    //TODO: refactor. Now clearing is simple brute force
    lcd_putstring(lcd, "                ");
    return_cursor_back();
}

/** clears bottom row content of lcd display */
static void clear_bottom_row(void)
{
    sk_lcd_cmd_setaddr(lcd, 0x40, false);
    //TODO: refactor. Now clearing is simple brute force
    lcd_putstring(lcd, "                ");
    return_cursor_back();
}

void print_welcome_msg(void)
{
    sk_lcd_cmd_onoffctl(lcd, true, false, false);
    sk_lcd_cmd_clear(lcd);
    lcd_putstring(lcd, "   Press OK");
    sk_lcd_cmd_setaddr(lcd, 0x40, false);
    lcd_putstring(lcd, " To enter pwd");
}

void print_no_pwd_msg(void)
{
    sk_lcd_cmd_clear(lcd);
    lcd_putstring(lcd, " Please set pwd");
    sk_lcd_cmd_setaddr(lcd, 0x40, false);
    lcd_putstring(lcd, " in master acc");
}

void print_wait_time(uint32_t time)
{
    char buf[16];
    snprintf(buf, sizeof(buf), "   %lu s", time);
    print_error(buf);
}

void print_info(const char *str)
{
    sk_lcd_cmd_onoffctl(lcd, true, false, false);
    clear_top_row();
    sk_lcd_cmd_setaddr(lcd, 0x00, false);
    lcd_putstring(lcd, str);
    return_cursor_back();
}

void print_error(const char *str)
{
    sk_lcd_cmd_onoffctl(lcd, true, false, false);
    clear_bottom_row();
    sk_lcd_cmd_setaddr(lcd, 0x40, false);
    lcd_putstring(lcd, str);
    return_cursor_back();
}

void draw_pwd_input(bool is_master)
{
    sk_lcd_cmd_onoffctl(lcd, true, true, false);
    sk_lcd_cmd_clear(lcd);
    lcd_menu.row = 0;
    lcd_menu.pwd_pos = 0;
    if (is_master) {
        //TODO: refactor. Now drawing is simple brute force
        lcd_putstring(lcd, "      ****       ");
        lcd_menu.pos = 6;
        return_cursor_back();
        show_sym(true);
    } else {
        lcd_putstring(lcd, "     ******      ");
        lcd_menu.pos = 5;
        return_cursor_back();
        show_sym(false);
    }
}

static void show_row_cursor(bool row)
{
    uint8_t addr = row ? 0x4F : 0x0F;
    uint8_t old_addr = row ? 0x0F : 0x4F;
    sk_lcd_cmd_setaddr(lcd, old_addr, false);
    sk_lcd_putchar(lcd, ' ');
    sk_lcd_cmd_setaddr(lcd, addr, false);
    sk_lcd_putchar(lcd, 'ґ');
}

void move_row_curs(void)
{
    lcd_menu.row = !lcd_menu.row;
    show_row_cursor(lcd_menu.row);
}

void print_options(void)
{
    sk_lcd_cmd_clear(lcd);
    lcd_putstring(lcd, " Change pwd");
    sk_lcd_cmd_setaddr(lcd, 0x40, false);
    lcd_putstring(lcd, " Exit");
    show_row_cursor(lcd_menu.row);
}

void print_next_options(void)
{
    sk_lcd_cmd_clear(lcd);
    lcd_putstring(lcd, " OPTION3");
    sk_lcd_cmd_setaddr(lcd, 0x40, false);
    lcd_putstring(lcd, " OPTION4");
    show_row_cursor(lcd_menu.row);
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
        sk_lcd_cmd_onoffctl(lcd, true, true, false);
    }
}

void move_cursor_right(bool is_master)
{
    uint8_t len = get_pwd_len(is_master);
    if(len - 1 != lcd_menu.pwd_pos) {
        lcd_menu.pos++;
        lcd_menu.pwd_pos++;
        sk_lcd_cmd_shift(lcd, false, true);
    } else {
        print_error("can't move right!");
        sk_tick_delay_ms(1500);
        clear_bottom_row();
        sk_lcd_cmd_onoffctl(lcd, true, true, false);
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


void open_lock(void)
{
    sk_pin_set(sk_io_led_red, false);
    sk_pin_set(sk_io_led_green, true);
    sk_lcd_cmd_clear(lcd);
    lcd_putstring(lcd, " Lock is opened!");
    sk_tick_delay_ms(1500);
    sk_lcd_cmd_clear(lcd);
    lcd_putstring(lcd, "    Press OK ");
    sk_lcd_cmd_setaddr(lcd, 0x40, false);
    lcd_putstring(lcd, " To lock it back");
}

void close_lock(void)
{
    sk_pin_set(sk_io_led_red, true);
    sk_pin_set(sk_io_led_green, false);
    sk_lcd_cmd_clear(lcd);
    lcd_putstring(lcd, " Lock is closed!");
    sk_tick_delay_ms(1500);
}
