#include "interrupts.h"

#include <stdio.h>

static extern struct sk_lcd lcd;

void handle_center_btn(void) {
    printf("Test\n");
	sk_lcd_cmd_onoffctl(&lcd, true, true, true);
    printf("after sk command\n");
    lcd_putstring(&lcd, "test");
}
