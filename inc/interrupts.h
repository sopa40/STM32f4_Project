#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "lcd_menu.h"
#include "lcd_hd44780.h"

void init_interrput_vars(void);

void exti0_isr(void);

void exti9_5_isr(void);

void exti15_10_isr(void);




#endif //INTERRUPTS_H
