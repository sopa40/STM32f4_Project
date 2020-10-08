#include "interrupts.h"
#include "lcd_menu.h"
#include "password.h"
#include <libopencm3/cm3/cortex.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/timer.h>

#include <stdio.h>

static struct sk_lcd *lcd = NULL;
static struct Menu *lcd_menu = NULL;

void init_interrput_vars(void)
{
    lcd = get_lcd();
	lcd_menu = get_lcd_menu();
}

static void handle_user_btn(void)
{
    draw_master_pass_input();
    switch(lcd_menu->status) {
        case MENU_INIT:
            lcd_menu->status = ENTER_MASTER_PASS;
            break;
        default:
            break;
    }
}

static void handle_center_btn(void)
{
    switch(lcd_menu->status) {
        case MENU_INIT:
            draw_pass_input();
            sk_lcd_cmd_onoffctl(lcd, true, true, false);
            lcd_menu->status = ENTER_PASS;
            break;
        case ENTER_PASS:
            if (is_pwd_correct(0)) {
                lcd_menu->status = ACCESS_GRANTED;
                sk_pin_toggle(sk_io_led_orange);
            } else
                lcd_menu->status = ACCESS_DENIED;
            break;
        case ENTER_MASTER_PASS:
            if (is_pwd_correct(1)) {
                lcd_menu->status = MASTER_ACCESS_GRANTED;
                sk_pin_toggle(sk_io_led_blue);
            } else
                lcd_menu->status = MASTER_ACCESS_DENIED;
            break;
        default:
            sk_lcd_cmd_clear(lcd);
            lcd_putstring(lcd, "some error");
            break;
    }
}

static void handle_left_btn(void)
{
    switch(lcd_menu->status) {
        case ENTER_PASS:
            hide_symbol();
            move_cursor_left();
            show_symbol(0);
            break;
        case ENTER_MASTER_PASS:
            hide_symbol();
            move_cursor_left();
            show_symbol(1);
            break;
        default:
            break;
    }
}

static void handle_right_btn(void)
{
    switch(lcd_menu->status) {
        case MENU_INIT:
            break;
        case ENTER_PASS:
            hide_symbol();
            move_cursor_right();
            show_symbol(0);
            break;
        case ENTER_MASTER_PASS:
            hide_symbol();
            move_cursor_right();
            show_symbol(1);
            break;
        default:
            break;
    }
}

static void handle_top_btn(void)
{
    char new_symb = 255;
    switch(lcd_menu->status) {
        case ENTER_PASS:
            new_symb = inc_value(lcd_menu->pass_symbol_pos, false);
            if(255 == new_symb)
                print_error("false inc symbol");
            sk_lcd_putchar(lcd, new_symb);
            sk_lcd_cmd_shift(lcd, false, false);
            break;
        case ENTER_MASTER_PASS:
            new_symb = inc_value(lcd_menu->pass_symbol_pos, true);
            if(255 == new_symb)
                print_error("false inc symbol");
            sk_lcd_putchar(lcd, new_symb);
            sk_lcd_cmd_shift(lcd, false, false);
            break;
        default:
            break;
    }sk_pin_toggle(sk_io_led_orange);
}

static void handle_bottom_btn(void)
{
    char new_symb = 255;
    switch(lcd_menu->status) {
        case ENTER_PASS:
            new_symb = dec_value(lcd_menu->pass_symbol_pos, false);
            if(255 == new_symb)
                print_error("false dec symbol");
            sk_lcd_putchar(lcd, new_symb);
            sk_lcd_cmd_shift(lcd, false, false);
            break;
        case ENTER_MASTER_PASS:
            new_symb = dec_value(lcd_menu->pass_symbol_pos, true);
            if(255 == new_symb)
                print_error("false dec symbol");
            sk_lcd_putchar(lcd, new_symb);
            sk_lcd_cmd_shift(lcd, false, false);
            break;
        default:
            break;
    }
}

//TODO: debounce buttons

void exti0_isr(void)
{
    printf("test\n");
    handle_user_btn();
    exti_reset_request(EXTI0);
}

//PC6 top
//PC8 bottom
//PC9 left
void exti9_5_isr(void)
{
	if (exti_get_flag_status(EXTI6)) {
		sk_pin_toggle(sk_io_led_red);
        handle_top_btn();
		exti_reset_request(EXTI6);
	}
	if (exti_get_flag_status(EXTI8)) {
		sk_pin_toggle(sk_io_led_green);
        handle_bottom_btn();
		exti_reset_request(EXTI8);
	}
	if (exti_get_flag_status(EXTI9)) {
        sk_pin_toggle(sk_io_led_orange);
		handle_left_btn();
		exti_reset_request(EXTI9);
	}
}

//PC11 right
//PA15 mid
void exti15_10_isr(void)
{
	if (exti_get_flag_status(EXTI11)) {
		sk_pin_toggle(sk_io_led_orange);
        handle_right_btn();
		exti_reset_request(EXTI11);
	}
	if (exti_get_flag_status(EXTI15)) {
		sk_pin_toggle(sk_io_led_blue);
		handle_center_btn();
		exti_reset_request(EXTI15);
	}
}
