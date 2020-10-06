#include "lcd_menu.h"
#include "interrupts.h"
#include "tick.h"
#include "spi.h"
#include <libopencm3/cm3/cortex.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/timer.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define MAX_PASS_LEN 6

#if defined(USE_SEMIHOSTING) && USE_SEMIHOSTING
	#include <stdio.h>
	extern void initialise_monitor_handles(void);
#endif

volatile uint8_t pass_sym_arr[MAX_PASS_LEN];

static struct sk_lcd *lcd = NULL;
static struct Menu *lcd_menu = NULL;



uint8_t read_status_reg = 0x05;




void init_variables(void)
{
	lcd = get_lcd();
	lcd_menu = get_lcd_menu();
	init_interrput_vars();
	init_menu_vars();
}


void init_exti_interrupt(uint32_t gpioport, uint16_t pin, enum exti_trigger_type trigger_type)
{
	exti_select_source(pin, gpioport);
	exti_set_trigger(pin, trigger_type);
	exti_enable_request(pin);
	exti_reset_request(pin);
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
		sk_lcd_cmd_shift(lcd, false, false);
		exti_reset_request(EXTI9);
	}
}

//PC11 right
//PA15 mid
void exti15_10_isr(void)
{
	if (exti_get_flag_status(EXTI11)) {
		sk_pin_toggle(sk_io_led_orange);
		sk_lcd_cmd_shift(lcd, false, true);
		exti_reset_request(EXTI11);
	}
	if (exti_get_flag_status(EXTI15)) {
		sk_pin_toggle(sk_io_led_blue);
		handle_center_btn();
		sk_lcd_cmd_shift(lcd, false, true);
		exti_reset_request(EXTI15);
	}
}

void init_interrupts(void)
{
	init_exti_interrupt(GPIOA, EXTI15, EXTI_TRIGGER_RISING);
	init_exti_interrupt(GPIOC, EXTI6, EXTI_TRIGGER_RISING);
	init_exti_interrupt(GPIOC, EXTI8, EXTI_TRIGGER_RISING);
	init_exti_interrupt(GPIOC, EXTI9, EXTI_TRIGGER_RISING);
	init_exti_interrupt(GPIOC, EXTI11, EXTI_TRIGGER_RISING);


	nvic_set_priority(NVIC_EXTI9_5_IRQ, 2 << 4);
	nvic_set_priority(NVIC_EXTI15_10_IRQ, 2 << 4);
	nvic_set_priority(NVIC_SYSTICK_IRQ, 1 << 4);

	nvic_enable_irq(NVIC_EXTI9_5_IRQ);
	nvic_enable_irq(NVIC_EXTI15_10_IRQ);

	cm_enable_interrupts();
}

void flash_test(void)
{
	write_enable();
	flash_show_status_reg();
    uint32_t addr = 0x0000FF;
    flash_write_byte(addr, 0x08);
	flash_show_status_reg();
    uint8_t test_byte = flash_read_byte(addr);

    char buffer[20];
    sk_lcd_cmd_setaddr(lcd, 0x00, false);
    snprintf(buffer, sizeof(buffer), "test:");
    lcd_putstring(lcd, buffer);

    sk_lcd_cmd_setaddr(lcd, 0x40, false);
    snprintf(buffer, sizeof(buffer), "%Xh", test_byte);
    lcd_putstring(lcd, buffer);

}


int main (void)
{
	initialise_monitor_handles();   		//for debugging
	init_variables();

 	clock_init();
 	spi_init();

	rcc_periph_clock_enable(RCC_GPIOD);		//LEDS
	rcc_periph_clock_enable(RCC_GPIOA);		//center btn
	rcc_periph_clock_enable(RCC_GPIOC);		//other btns
	rcc_periph_clock_enable(RCC_GPIOE);		//LCD display
	rcc_periph_clock_enable(RCC_SYSCFG);	//for EXTI interrupts

	glsk_pins_init(false);
	/* configuration start */
	sk_pin_set(sk_io_led_red, true);

	init_interrupts();

	sk_tick_init(16000000ul / 10000ul);



	sk_pin_group_set(sk_io_lcd_data, 0x00);
	sk_lcd_init(lcd);
	sk_lcd_cmd_onoffctl(lcd, true, false, false);
	sk_lcd_set_backlight(lcd, 200);
	sk_lcd_cmd_setaddr(lcd, 0x00, false);
	// lcd_putstring(lcd, "   Press OK");
	// sk_lcd_cmd_setaddr(lcd, 0x40, false);
	// lcd_putstring(lcd, " To enter pass1");

	flash_show_status_reg();
	write_enable();
	flash_show_status_reg();
	flash_show_status_reg();
	flash_test();






	/* configuration successful end */
	sk_pin_set(sk_io_led_red, false);

	while (1) {

	}
}
