#include "lcd_menu.h"
#include "interrupts.h"
#include "tick.h"
#include "spi.h"
#include "password.h"
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

#if defined(USE_SEMIHOSTING) && USE_SEMIHOSTING
	#include <stdio.h>
	extern void initialise_monitor_handles(void);
#endif

static struct sk_lcd *lcd = NULL;
static struct Menu *lcd_menu = NULL;

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

void init_interrupts(void)
{
	init_exti_interrupt(GPIOA, EXTI15, EXTI_TRIGGER_RISING);
	init_exti_interrupt(GPIOA, EXTI0, EXTI_TRIGGER_RISING);
	init_exti_interrupt(GPIOC, EXTI6, EXTI_TRIGGER_RISING);
	init_exti_interrupt(GPIOC, EXTI8, EXTI_TRIGGER_RISING);
	init_exti_interrupt(GPIOC, EXTI9, EXTI_TRIGGER_RISING);
	init_exti_interrupt(GPIOC, EXTI11, EXTI_TRIGGER_RISING);


	nvic_set_priority(NVIC_SPI1_IRQ, 0 << 4);
	nvic_set_priority(NVIC_SYSTICK_IRQ, 1 << 4);
	nvic_set_priority(NVIC_EXTI0_IRQ, 2 << 4);
	nvic_set_priority(NVIC_EXTI9_5_IRQ, 3 << 4);
	nvic_set_priority(NVIC_EXTI15_10_IRQ, 3 << 4);

	nvic_enable_irq(NVIC_EXTI0_IRQ);
	nvic_enable_irq(NVIC_EXTI9_5_IRQ);
	nvic_enable_irq(NVIC_EXTI15_10_IRQ);

	cm_enable_interrupts();
}



int main (void)
{
	initialise_monitor_handles();   		//for debugging
	init_variables();
 	clock_init();
 	spi_init();
	rcc_periph_clock_enable(RCC_GPIOD);		//LEDS
	rcc_periph_clock_enable(RCC_GPIOA);		//center btn and user btn
	rcc_periph_clock_enable(RCC_GPIOC);		//other btns
	rcc_periph_clock_enable(RCC_GPIOE);		//LCD display
	rcc_periph_clock_enable(RCC_SYSCFG);	//for EXTI interrupts

	glsk_pins_init(false);
	/* configuration start */
	sk_pin_set(sk_io_led_blue, true);

	sk_tick_init(16000000ul / 10000ul);

	init_lcd_with_settings();

	flash_unlock();
	flash_write_byte(FIRST_ADDR_DEFAULT_M_PWD, '1');
	flash_write_byte(FIRST_ADDR_DEFAULT_M_PWD + 1, '0');
	flash_write_byte(FIRST_ADDR_DEFAULT_M_PWD + 2, '1');
	flash_write_byte(FIRST_ADDR_DEFAULT_M_PWD + 3, '0');
	flash_lock();

	if (!init_pwd_settings())
		lcd_menu->status = PWD_NOT_SET;

	init_interrupts();
	/* configuration successful end */
	sk_pin_set(sk_io_led_blue, false);
	sk_pin_set(sk_io_led_red, true);

	while (1) {

	}
}
