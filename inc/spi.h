#ifndef SPI_H
#define SPI_H

#include "password.h"

//TODO: add function to write more than one byte at a time
//      add error codes to some function

#define NOT_AVAILABLE_ADDR 0x1FFFFF

//how many data save during page clearing
#define DATA_TO_SAVE 10

#define ONE_ADDR 0x000001
#define ADDR_MASTER_PASS_LEN ONE_ADDR * MASTER_PASS_LEN
#define ADDR_PASS_LEN ONE_ADDR * PASS_LEN

//4kbytes for m_pwd.is_set
#define FIRST_ADDR_MS_PWD_IS_SET 0x1F7FFF
#define LAST_ADDR_MS_PWD_IS_SET 0x1FFFFF - ONE_ADDR

//4kbytes for pwd.is_set
#define FIRST_ADDR_PWD_IS_SET 0x1F0000
#define LAST_ADDR_PWD_IS_SET 0x1F7FFF - ONE_ADDR

//64kbytes for m_pwd.val
#define FIRST_ADDR_MS_PWD 0x170000
#define LAST_ADDR_MS_PWD 0x1F0000 - ONE_ADDR

//64kbytes for pwd.val
#define FIRST_ADDR_PWD 0x0F0000
#define LAST_ADDR_PWD 0x170000 - ONE_ADDR

//4kbytes for default master pass
#define FIRST_ADDR_DEFAULT_M_PWD 0x0E8000
#define LAST_ADDR_DEFAULT_M_PWD 0x0F0000 - ONE_ADDR

//4kbytes for rewriting pages
#define FIRST_ADDR_TEMP_SAVE 0x0E0000
#define LAST_ADDR_TEMP_SAVE 0x0E8000 - ONE_ADDR

//32kbytes for pwd_attempts
#define FIRST_ADDR_ATTEMPTS 0x0A0000
#define LAST_ADDR_ATTEMPTS 0x0E0000 - ONE_ADDR

//32kbytes for pwd_delay
#define FIRST_ADDR_DELAY 0x060000
#define LAST_ADDR_DELAY 0x0A0000 - ONE_ADDR

//4kbytes for m_pwd_attempts
#define FIRST_ADDR_M_ATTEMPTS 0x058000
#define LAST_ADDR_M_ATTEMPTS 0x60000 - ONE_ADDR

//4kbytes for m_pwd_delay
#define FIRST_ADDR_M_DELAY 0x050000
#define LAST_ADDR_M_DELAY 0x058000 - ONE_ADDR


void spi_init(void);

void clock_init(void);

void flash_tx(uint32_t len, const void *data);

void flash_rx(uint32_t len, const void *data);

void cs_set(bool state);

void write_enable(void);

void write_disable(void);

void flash_unlock(void);

void flash_lock(void);

bool is_busy(void);

uint8_t flash_show_status_reg(void);

void flash_write_byte(uint32_t addr, uint8_t data);

uint8_t flash_read_byte(uint32_t addr);

void flash_erase_4kb(uint32_t start_addr);

void flash_erase_32kb(uint32_t start_addr);

void flash_erase_64kb(uint32_t start_addr);

void flash_erase_full(void);

void clear_page(uint32_t start_addr, uint32_t end_addr);

uint32_t find_free_addr(uint32_t start_addr, uint32_t end_addr);


#endif //SPI_H
