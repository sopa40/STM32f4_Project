#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "lcd_menu.h"
#include "lcd_hd44780.h"

/*!
*
*	\brief 					Initializes lcd and lcd_menu variables
*
*/
void init_interrput_vars(void);

/*!
*
*	\brief 					exti0 interrupt routine
*
*/
void exti0_isr(void);

/*!
*
*	\brief 					exti9_5 interrupt routine
*
*/
void exti9_5_isr(void);

/*!
*
*	\brief 					exti15_10 interrupt routine
*
*/
void exti15_10_isr(void);




#endif //INTERRUPTS_H
