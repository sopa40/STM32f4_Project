#ifndef SPI_H
#define SPI_H

#include "password.h"

//TODO: add function to write more than one byte at a time
//      add error codes to some function

#define NOT_AVAILABLE_ADDR 0x1FFFFF

/** Data to be saved during page clearing */
#define DATA_TO_SAVE 10

#define ONE_ADDR 0x000001
#define ADDR_MASTER_PASS_LEN ONE_ADDR * MASTER_PASS_LEN
#define ADDR_PASS_LEN ONE_ADDR * PASS_LEN

/**  4kbytes for m_pwd.is_set */
#define FIRST_ADDR_MS_PWD_IS_SET 0x1F7FFF
#define LAST_ADDR_MS_PWD_IS_SET 0x1FFFFF - ONE_ADDR

/** 4kbytes for pwd.is_set */
#define FIRST_ADDR_PWD_IS_SET 0x1F0000
#define LAST_ADDR_PWD_IS_SET 0x1F7FFF - ONE_ADDR

/** 64kbytes for m_pwd.val */
#define FIRST_ADDR_MS_PWD 0x170000
#define LAST_ADDR_MS_PWD 0x1F0000 - ONE_ADDR

/** 64kbytes for pwd.val */
#define FIRST_ADDR_PWD 0x0F0000
#define LAST_ADDR_PWD 0x170000 - ONE_ADDR

/** 4kbytes for default master pass */
#define FIRST_ADDR_DEFAULT_M_PWD 0x0E8000
#define LAST_ADDR_DEFAULT_M_PWD 0x0F0000 - ONE_ADDR

/** 4kbytes for rewriting pages */
#define FIRST_ADDR_TEMP_SAVE 0x0E0000
#define LAST_ADDR_TEMP_SAVE 0x0E8000 - ONE_ADDR

/** 32kbytes for pwd_attempts */
#define FIRST_ADDR_ATTEMPTS 0x0A0000
#define LAST_ADDR_ATTEMPTS 0x0E0000 - ONE_ADDR

/** 32kbytes for pwd_delay */
#define FIRST_ADDR_DELAY 0x060000
#define LAST_ADDR_DELAY 0x0A0000 - ONE_ADDR

/** 4kbytes for m_pwd_attempts */
#define FIRST_ADDR_M_ATTEMPTS 0x058000
#define LAST_ADDR_M_ATTEMPTS 0x60000 - ONE_ADDR

/** 4kbytes for m_pwd_delay */
#define FIRST_ADDR_M_DELAY 0x050000
#define LAST_ADDR_M_DELAY 0x058000 - ONE_ADDR


/*!
*
*	\brief 					Initializes clock
*
*/
void clock_init(void);

/*!
*
*	\brief 					Initializes SPI1
*
*/
void spi_init(void);

/*!
*	\brief 					Unlocks flash for writing
*
*/
void flash_unlock(void);

/*!
*	\brief 					Locks flash for writing
*
*/
void flash_lock(void);

/*!
*	\brief 					Writes one byte of data to the Flash memory
*
*	\warning				Flash should be unlocked before and locked after
*
*	\param[addr]			Address in the Flash where data should be written to
*
*	\param[data]			Data to be written to the Flash
*
*/
void flash_write_byte(uint32_t addr, uint8_t data);

/*!
*	\brief 					Writes one byte of data to the Flash memory
*
*	\param[addr]			Address in the Flash where data should be written from
*
*	\return					Returns byte data that was read from Flash
*
*/
uint8_t flash_read_byte(uint32_t addr);

/*!
*	\brief 					Clears memory segment
*
*	\warning 				If memory length (diff between start and end addr) does not correspond
*							with possible memory erase functions, prints error to the LCD
*
*
*	\param[start_addr]		Start address of the memory segment to be cleared
*
*	\param[end_addr]		End address of the memory segment to be cleared
*
*	\return					Returns byte data that was read from Flash
*
*/
void clear_page(uint32_t start_addr, uint32_t end_addr);

/*!
*	\brief 					Finds first free address
*
*   \warning                Free adress means address with 0xFF value
*
*	\param[start_addr]		Start address of the memory segment where free addr should be found
*
*	\param[end_addr]		End address of the memory segment where free addr should be found
*
*	\return					Returns address first free address in interval. If not address found, returns NOT_AVAILABLE_ADDR (0x1FFFFF)
*
*/
uint32_t find_free_addr(uint32_t start_addr, uint32_t end_addr);


#endif //SPI_H
