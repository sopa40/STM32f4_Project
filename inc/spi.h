#ifndef SPI_H
#define SPI_H

void spi_init(void);

void clock_init(void);

void flash_tx(uint32_t len, const void *data);

void flash_rx(uint32_t len, void *data);

void cs_set(bool state);

void write_enable(void);

uint8_t get_status_register(void);

void unlock_flash_for_writing(void);

void lock_flash_for_writing(void);

void write_byte(uint32_t addr, uint8_t data);

uint8_t read_byte(uint32_t addr);

void read_data(uint32_t addr, uint32_t size, const void *data);

#endif //SPI_H
