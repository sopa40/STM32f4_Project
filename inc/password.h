#ifndef PASSWORD_H
#define PASSWORD_H

#include "lcd_menu.h"
#include "tick.h"
#include "spi.h"
#include <stdbool.h>
#include <stdint.h>

//TODO: add pwd len setting in master menu
/*!
*	\brief 			defines master and passlen. Should be later defined in master
*
*	\warning		master pwd len must not be longer than pwd len
*/
#define MASTER_PASS_LEN 4
#define PASS_LEN 6
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

/*!
*	\brief					Clears value of input in new password
*
*	\param[is_master]		Determines if master password or simple pwd
*
*/
void clear_pwd_input(bool is_master);

/*!
*	\brief 					Loads master password and simple password.
*
*	\warning 				Fatal fails are not handeled correctly. But still detected
*
*	\return					False if something went wrong. For example, password is not in memory
*/
bool init_pwd_settings(void);

/*!
*	\brief 					Returns new_password symbol in current position
*
*	\param[pos]				Position in the password to be returned
*
*	\param[is_master]		Determines if master password or simple pwd
*
*	\return					255 if position bigger than max pass len. Otherwise password symbol
*/
char get_pwd_sym(uint8_t pos, bool is_master);

/*!
*	\brief 					Returns password length
*
*	\param[is_master]		Determines if master password or simple pwd
*
*	\return					Password length, set in the struct Password
*/
uint8_t get_pwd_len(bool is_master);

/*!
*	\brief 					Increases value of new_password
*
*	\param[pos]				Position in the password to be changed
*
*	\param[is_master]		Determines if master password or simple pwd
*
*	\return					255 if position bigger than max pass len. Otherwise password symbol that was changed
*/
char inc_value(uint8_t pos, bool is_master);

/*!
*	\brief 					Decreases value of new_password
*
*	\param[pos]				Position in the password to be changed
*
*	\param[is_master]		Determines if master password or simple pwd
*
*	\return					255 if position bigger than max pass len. Otherwise password symbol that was changed
*/
char dec_value(uint8_t pos, bool is_master);

/*!
*	\brief 					Checks if new password and origin password (loaded from memory) equal
*
*	\param[is_master]		Determines if master password or simple pwd
*
*	\return					False if password are not equal or origin password is not initiated (value from struct). Otherwise true
*/
bool is_pwd_eq(bool is_master);

/*!
*	\brief 					Uploads new password in the corresponding memory segment. Clears segment if needed
*
*	\param[is_master]		Determines if master password or simple pwd
*
*/
void save_pwd(bool is_master);

/*!
*	\brief 					Descreases number of password attempts and writes new value to the memory
*
*	\param[is_master]		Determines if master password or simple pwd
*
*/
void dec_attempts(bool is_master);

/*!
*	\brief 					Restores number of password attempts and writes new value to the memory
*
*	\param[is_master]		Determines if master password or simple pwd
*
*/
void restore_attempts(bool is_master);

/*!
*	\brief 					Gets number of password attempts
*
*	\param[is_master]		Determines if master password or simple pwd
*
*	\return					Returns password attempts
*/
uint8_t get_attempts(bool is_master);

/*!
*	\brief 					Loads current amount of time from memory and waits it. In the end adds attempt
*
*	\param[is_master]		Determines if master password or simple pwd
*
*/
void wait_to_try(bool is_master);

/*!
*
*	\brief 					Prints master password. Insecure, for debugging goals
*
*/
//void get_m_pwd(void);

/*!
*
*	\brief 					Prints master password. Insecure, for debugging goals
*
*/
//void get_pwd(void);


#endif // PASSWORD_H
