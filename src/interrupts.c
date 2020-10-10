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
    switch(lcd_menu->status) {
        case ENTER_MASTER_PASS:
            break;
        default:
            clear_pwd_input(true);
            draw_pwd_input(true);
            lcd_menu->status = ENTER_MASTER_PASS;
            break;
    }
}

static void handle_center_btn(void)
{
    switch(lcd_menu->status) {
        case MENU_INIT:
            clear_pwd_input(false);
            draw_pwd_input(false);
            sk_lcd_cmd_onoffctl(lcd, true, true, false);
            lcd_menu->status = ENTER_PASS;
            break;
        case ENTER_PASS:
            if (is_pwd_correct(0)) {
                lcd_menu->status = ACCESS_GRANTED;
                sk_lcd_cmd_clear(lcd);
                print_error("It is open");
                sk_pin_toggle(sk_io_led_orange);
            } else
                lcd_menu->status = ACCESS_DENIED;
            break;
        case ENTER_MASTER_PASS:
            if (is_pwd_correct(1)) {
                lcd_menu->status = MASTER_ACCESS_GRANTED;
                sk_lcd_cmd_clear(lcd);
                lcd_putstring(lcd, "WELCOME TO MENU");
                sk_tick_delay_ms(1500);
                lcd_menu->status = OPTIONS1;
                print_options();
            } else {
                lcd_menu->status = MASTER_ACCESS_DENIED;
                print_error("false master pwd");
            }
            break;
        case CHANGE_PWD:
            //here correct means same pwd input
            if (!is_pwd_correct(0)) {
                save_pwd(0);
            }
            else {
                print_error("Same pwd, change");
            }
            break;
        case OPTIONS1:
            if (!lcd_menu->row) {
                lcd_menu->state = CHANGE_PWD;
                draw_pwd_input(false);
            }
            else {
                //handle second option
            }
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
            hide_sym();
            move_cursor_left();
            show_sym(0);
            break;
        case ENTER_MASTER_PASS:
            hide_sym();
            move_cursor_left();
            show_sym(1);
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
            hide_sym();
            move_cursor_right();
            show_sym(0);
            break;
        case ENTER_MASTER_PASS:
            hide_sym();
            move_cursor_right();
            show_sym(1);
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
            new_symb = inc_value(lcd_menu->pwd_pos, false);
            if(255 == new_symb)
                print_error("false inc symbol");
            sk_lcd_putchar(lcd, new_symb);
            sk_lcd_cmd_shift(lcd, false, false);
            break;
        case ENTER_MASTER_PASS:
            new_symb = inc_value(lcd_menu->pwd_pos, true);
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
            new_symb = dec_value(lcd_menu->pwd_pos, false);
            if(255 == new_symb)
                print_error("false dec symbol");
            sk_lcd_putchar(lcd, new_symb);
            sk_lcd_cmd_shift(lcd, false, false);
            break;
        case ENTER_MASTER_PASS:
            new_symb = dec_value(lcd_menu->pwd_pos, true);
            if(255 == new_symb)
                print_error("false dec symbol");
            sk_lcd_putchar(lcd, new_symb);
            sk_lcd_cmd_shift(lcd, false, false);
            break;
        case OPTIONS1:
            if(!lcd_menu->row) {
                lcd_menu->row = 1;
                //go to second option
            } else {
                lcd_menu->status = OPTIONS2;
                lcd_menu->row = 0;
                //go to next option list
            }
            break;
        default:
            break;
    }
}

//TODO: debounce buttons
//PA0 user
void exti0_isr(void)
{
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
