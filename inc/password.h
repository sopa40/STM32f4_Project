#ifndef PASSWORD_H
#define PASSWORD_H

#include <stdbool.h>
#include <stdint.h>

//TODO: add pass len setting in master menu
#define PASS_LEN 6

struct Password {
	uint8_t pass_len;
	volatile uint8_t values[PASS_LEN];
	bool is_set;
};

struct Password *get_password(void);

char get_pass_symbol (uint8_t position);

uint8_t get_pass_len(void);

char inc_value(uint8_t position);

char dec_value(uint8_t position);

bool is_pwd_correct(void);


#endif // PASSWORD_H
