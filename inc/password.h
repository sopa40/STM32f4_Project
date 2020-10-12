#ifndef PASSWORD_H
#define PASSWORD_H

#include "lcd_menu.h"
#include "tick.h"
#include "spi.h"
#include <stdbool.h>
#include <stdint.h>

//TODO: add pwd len setting in master menu
//WARNING! master pwd len must not be longer than pwd len
#define PASS_LEN 6
#define MASTER_PASS_LEN 4
#define DELAY_FAC 2

struct Password {
	uint8_t pwd_len;
	uint8_t val[PASS_LEN];
	uint8_t attempts;
	bool is_set;
	bool is_init;
};

struct New_Password {
	uint8_t pwd_len;
	uint8_t val[PASS_LEN];
};

void clear_pwd_input(bool is_master);

bool init_pwd_settings(void);

char get_pwd_sym(uint8_t position, bool is_master);

uint8_t get_pwd_len(bool is_master);

char inc_value(uint8_t pos, bool is_master);

char dec_value(uint8_t pos, bool is_master);

bool is_pwd_eq(bool is_master);

bool is_master_pwd_set(void);

void save_pwd(bool is_master);

void change_pwd(bool is_master);

void dec_attempts(bool is_master);

void restore_attempts(bool is_master);

uint8_t get_attempts(bool is_master);

void wait_to_try(bool is_master);

void get_m_pwd(void);

void get_pwd(void);


#endif // PASSWORD_H
