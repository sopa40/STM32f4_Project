#include "interrupts.h"

#include <stdio.h>

struct sk_lcd *lcd = get_lcd();

void handle_center_btn(void) {
    printf("Test\n");
	sk_lcd_cmd_onoffctl(lcd, true, true, true);
    lcd_putstring(lcd, "test");
}
