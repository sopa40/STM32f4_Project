#include "interrupts.h"
#include "lcd_menu.h"
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

void handle_center_btn(void)
{
    switch(lcd_menu->status) {
        case MENU_INIT:
            draw_pass_input();
            lcd_menu->status = ENTER_PASS;
            break;
        case ENTER_PASS:
            print_error("Please enter pass");
            break;
        default:
            sk_lcd_cmd_clear(lcd);
            lcd_putstring(lcd, "some error");
            break;
    }
	sk_lcd_cmd_onoffctl(lcd, true, true, true);
}

void handle_left_btn(void)
{
    switch(lcd_menu->status) {
        case ENTER_PASS:
            move_cursor_left();
            break;
        default:
            break;
    }
}

void handle_right_btn(void)
{
    switch(lcd_menu->status) {
        case ENTER_PASS:
            move_cursor_right();
            break;
        default:
            break;
    }
}

//TODO: debounce buttons
//PC6 top
//PC8 bottom
//PC9 left
void exti9_5_isr(void)
{
	if (exti_get_flag_status(EXTI6)) {
		sk_pin_toggle(sk_io_led_red);
		exti_reset_request(EXTI6);
	}
	if (exti_get_flag_status(EXTI8)) {
		sk_pin_toggle(sk_io_led_green);
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
