#ifndef SPI_H
#define SPI_H

#include "password.h"

#define NOT_AVAILABLE_ADDR 0x1FFFFF

#define ONE_ADDR 0x000001
#define ADDR_MASTER_PASS_LEN ONE_ADDR * MASTER_PASS_LEN
#define ADDR_PASS_LEN ONE_ADDR * PASS_LEN

//4kbytes for master_password.is_set
#define FIRST_ADDR_MS_PWD 0x1F7FFF
#define LAST_ADDR_MS_PWD 0x1FFFFF - 0x000001

#define FIRST_ADDR_PWD 0x1F00000
#define LAST_ADDR_PWD 0x1F7FFF - 0x000001


void spi_init(void);

void clock_init(void);

void flash_tx(uint32_t len, const void *data);

void flash_rx(uint32_t len, void *data);

void cs_set(bool state);

void write_enable(void);

void write_disable(void);

void flash_unlock(void);

void flash_lock(void);

bool is_busy(void);

void flash_show_status_reg(void);

void flash_write_byte(uint32_t addr, uint8_t data);

int8_t flash_read_byte(uint32_t addr);

void flash_erase_full(void);

uint32_t find_free_addr(uint32_t start_addr, uint32_t end_addr);


#endif //SPI_H
