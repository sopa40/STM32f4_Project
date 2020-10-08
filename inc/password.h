#ifndef PASSWORD_H
#define PASSWORD_H

#include <stdbool.h>
#include <stdint.h>

//TODO: add pass len setting in master menu
#define PASS_LEN 6
#define MASTER_PASS_LEN 4

struct Password {
	uint8_t pass_len;
	uint8_t values[PASS_LEN];
	bool is_set;
};

void init_pwd_settings(void);

char get_pass_symbol (uint8_t position, bool is_master);

uint8_t get_pass_len(void);

char inc_value(uint8_t position, bool is_master);

char dec_value(uint8_t position, bool is_master);

bool is_pwd_correct(bool is_master);

bool is_master_pwd_set(void);


#endif // PASSWORD_H
