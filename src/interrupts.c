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
        case ENTER_MASTER_PWD:
            break;
        default:
            clear_pwd_input(true);
            draw_pwd_input(true);
            lcd_menu->status = ENTER_MASTER_PWD;
            break;
    }
}

static void handle_center_btn(void)
{
    switch(lcd_menu->status) {
        case MENU_INIT:
            lcd_menu->status = ENTER_PWD;
            clear_pwd_input(false);
            draw_pwd_input(false);
            sk_lcd_cmd_onoffctl(lcd, true, true, false);
            break;
        case ENTER_PWD:
            if (is_pwd_eq(0) && !lcd_menu->is_in_master) {
                lcd_menu->status = ACCESS_GRANTED;
                restore_attempts(false);
                sk_lcd_cmd_onoffctl(lcd, true, false, false);
                open_lock();
            } else if (!is_pwd_eq(0) && lcd_menu->is_in_master) {
                lcd_menu->status = OPTIONS1;
                save_pwd(0);
                restore_attempts(false);
                sk_lcd_cmd_onoffctl(lcd, true, false, false);
                print_error(" ");
                print_info("   pwd changed");
                sk_tick_delay_ms(1500);
                print_options();
            } else {
                if (!lcd_menu->is_in_master) {
                    uint8_t attempts = get_attempts(false);
                    if (attempts <= 3 && attempts > 1) {
                        char buff[16];
                        dec_attempts(false);
                        attempts--;
                        print_error(" Input is wrong");
                        sk_tick_delay_ms(1000);
                        snprintf(buff, sizeof(buff), "%u more attempts", attempts);
                        print_error(buff);
                    } else if (attempts == 1) {
                        dec_attempts(false);
                        attempts--;
                        wait_to_try(false);
                        lcd_menu->status = ENTER_PWD;
                    }
                    else
                        print_error("bad attempts value");
                }
                else {
                    print_error("same pwd, change");
                }
            }
            break;
        case ENTER_MASTER_PWD:
            if (is_pwd_eq(1)) {
                lcd_menu->status = MASTER_ACCESS_GRANTED;
                restore_attempts(true);
                sk_lcd_cmd_clear(lcd);
                print_info("Welcome to menu");
                sk_tick_delay_ms(1500);
                lcd_menu->status = OPTIONS1;
                lcd_menu->is_in_master = true;
                print_options();
            } else {
                uint8_t attempts = get_attempts(true);
                if (attempts <= 3 && attempts > 1) {
                    char buff[16];
                    dec_attempts(true);
                    attempts--;
                    print_error(" Input is wrong");
                    sk_tick_delay_ms(1000);
                    snprintf(buff, sizeof(buff), "%u more attempts", attempts);
                    print_error(buff);
                } else if (attempts == 1) {
                    dec_attempts(true);
                    attempts--;
                    wait_to_try(true);
                    lcd_menu->status = ENTER_MASTER_PWD;
                }
                else
                    print_error("bad attempts value");
            }
            break;
        case ACCESS_GRANTED:
            lcd_menu->status = MENU_INIT;
            lcd_menu->is_in_master = false;
            close_lock();
            print_welcome_msg();
            break;
        case OPTIONS1:
            if (!lcd_menu->row) {
                lcd_menu->status = ENTER_PWD;
                clear_pwd_input(false);
                draw_pwd_input(false);
            }
            else {
                lcd_menu->status = MENU_INIT;
                lcd_menu->is_in_master = false;
                print_welcome_msg();
            }
            break;
        default:
            sk_lcd_cmd_clear(lcd);
            printf("status is %u\n", lcd_menu->status);
            lcd_putstring(lcd, "some error");
            break;
    }
}

static void handle_left_btn(void)
{
    switch(lcd_menu->status) {
        case ENTER_PWD:
            hide_sym();
            move_cursor_left();
            show_sym(0);
            break;
        case ENTER_MASTER_PWD:
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
    switch (lcd_menu->status) {
        case MENU_INIT:
            break;
        case ENTER_PWD:
            hide_sym();
            move_cursor_right(false);
            show_sym(0);
            break;
        case ENTER_MASTER_PWD:
            hide_sym();
            move_cursor_right(true);
            show_sym(1);
            break;
        default:
            break;
    }
}

static void handle_top_btn(void)
{
    char new_symb = 255;
    switch (lcd_menu->status) {
        case ENTER_PWD:
            new_symb = inc_value(lcd_menu->pwd_pos, false);
            if (255 == new_symb)
                print_error("false inc symbol");
            sk_lcd_putchar(lcd, new_symb);
            sk_lcd_cmd_shift(lcd, false, false);
            break;
        case ENTER_MASTER_PWD:
            new_symb = inc_value(lcd_menu->pwd_pos, true);
            if (255 == new_symb)
                print_error("false inc symbol");
            sk_lcd_putchar(lcd, new_symb);
            sk_lcd_cmd_shift(lcd, false, false);
            break;
        case OPTIONS1:
            move_row_curs();
            break;
        case OPTIONS2:
            if(!lcd_menu->row) {
                lcd_menu->status = OPTIONS1;
                print_options();
            }
            move_row_curs();
            break;
        default:
            break;
    }
}

static void handle_bottom_btn(void)
{
    char new_symb = 255;
    switch(lcd_menu->status) {
        case ENTER_PWD:
            new_symb = dec_value(lcd_menu->pwd_pos, false);
            if(new_symb == 255)
                print_error("false dec symbol");
            sk_lcd_putchar(lcd, new_symb);
            sk_lcd_cmd_shift(lcd, false, false);
            break;
        case ENTER_MASTER_PWD:
            new_symb = dec_value(lcd_menu->pwd_pos, true);
            if(new_symb == 255)
                print_error("false dec symbol");
            sk_lcd_putchar(lcd, new_symb);
            sk_lcd_cmd_shift(lcd, false, false);
            break;
        case OPTIONS1:
            if(lcd_menu->row) {
                //go to second option list
                lcd_menu->status = OPTIONS2;
                print_next_options();
            }
            move_row_curs();
            break;
        case OPTIONS2:
            move_row_curs();
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
        handle_top_btn();
		exti_reset_request(EXTI6);
	}
	if (exti_get_flag_status(EXTI8)) {
        handle_bottom_btn();
		exti_reset_request(EXTI8);
	}
	if (exti_get_flag_status(EXTI9)) {
		handle_left_btn();
		exti_reset_request(EXTI9);
	}
}

//PC11 right
//PA15 mid
void exti15_10_isr(void)
{
	if (exti_get_flag_status(EXTI11)) {
        handle_right_btn();
		exti_reset_request(EXTI11);
	}
	if (exti_get_flag_status(EXTI15)) {
		handle_center_btn();
		exti_reset_request(EXTI15);
	}
}










// static uint8_t plen [2] = {PASS_LEN, MASTER_PASS_LEN};
// static uint8_t *pptr [2] = {new_pwd.val, new_m_pwd.val};
//
// char dec_value(uint8_t pos, bool is_master)
// {
//     uint8_t len = plen[is_master];
//     uint8_t *p = pptr[is_master];
//
//     if (pos >= len)
//         return 255;
//
//     p[pos]--;
//
//     if (p[pos] < '0')
//         p[pos] = '9';
//
//     return p[pos];
// }
//
// char inc_value(uint8_t pos, bool is_master)
// {
//     uint8_t len = plen[is_master];
//     uint8_t *p = pptr[is_master];
//
//     if (pos >= len)
//         return 255;
//
//     p[pos]++;
//
//     if (p[pos] > '9')
//         p[pos] = '0';
//
//     return p[pos];
// }
//
// ---------------------------------------------
// static uint8_t plen [2] = {PASS_LEN, MASTER_PASS_LEN};
// static uint8_t *pptr [2] = {new_pwd.val, new_m_pwd.val};
//
// char handle_pos_move(uint8_t pos, bool is_master, bool is_inc) {
//
// 	uint8_t len = plen[is_master];
//     uint8_t *p = pptr[is_master];
//
// 	uint8_t diff = is_inc ? 10 : (uint8_t)(-10)
// 	uint8_t inc = is_inc ? 1 : (uint8_t)(-1)
//
//     if (pos >= len)
//         return 255;
//
// 	p[pos] += inc;
//
// 	if (p[pos] > '9' || p[pos] < '0')
// 		p[pos] += diff
//
// 	return p[pos]
// }
//
// char inc_value(uint8_t pos, bool is_master) {
// 	return handle_pos_move(pos, is_master, true);
// }
//
// char dec_value(uint8_t pos, bool is_master) {
// 	return handle_pos_move(pos, is_master, false);
// }
