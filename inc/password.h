#ifndef PASSWORD_H
#define PASSWORD_H

#include <stdbool.h>
#include <stdint.h>

//TODO: add pwd len setting in master menu
//WARNING!! master pwd len should not be longer than pwd len
#define PASS_LEN 6
#define MASTER_PASS_LEN 4

struct Password {
	uint8_t pwd_len;
	uint8_t val[PASS_LEN];
	bool is_set;
	bool is_init;
};

struct New_Password {
	uint8_t pwd_len;
	uint8_t val[PASS_LEN];
};

void init_pwd_settings(void);

char get_pwd_sym(uint8_t position, bool is_master);

uint8_t get_pwd_len(void);

char inc_value(uint8_t pos, bool is_master);

char dec_value(uint8_t pos, bool is_master);

bool is_pwd_correct(bool is_master);

bool is_master_pwd_set(void);

void get_m_pwd(void);

void get_pwd(void);


#endif // PASSWORD_H
