#ifndef PASSWORD_H
#define PASSWORD_H


struct Password {
	uint8_t pass_len;
	volatile uint8_t values[PASS_LEN];
	bool is_set;
};

struct Password *get_password(void);

#endif // PASSWORD_H
