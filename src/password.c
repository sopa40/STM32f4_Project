#include "password.h"
#include "spi.h"

#include <stdio.h>

static struct Password master_password = {
    .pass_len = MASTER_PASS_LEN,
    .values = {'0', '1', '0', '1'},
    .is_set = false
};

static struct Password password = {
    .pass_len = PASS_LEN,
    .values = {255, 255, 255, 255, 255, 255},
    .is_set = false
};


static struct Password new_master_password = {
    .pass_len = MASTER_PASS_LEN,
    .values = {'0', '0', '0', '0'},
    .is_set = false
};

static struct Password new_password = {
    .pass_len = PASS_LEN,
    .values = {'0', '0', '0', '0', '0', '0'},
    .is_set = false
};



static uint32_t _get_pwd_start_addr(uint32_t start_addr, uint32_t end_addr, uint8_t pwd_len)
{
    uint32_t addr = find_free_addr(start_addr, end_addr);
    printf("first free addr is %lu\n", addr);
    if(start_addr == addr || NOT_AVAILABLE_ADDR == addr)
        return NOT_AVAILABLE_ADDR;
    if(addr - pwd_len < start_addr)
        return NOT_AVAILABLE_ADDR;
    return addr - pwd_len;
}

static void _load_pwd(uint32_t start_addr, uint8_t pwd_len, struct Password *pass)
{
    for(uint32_t i = 0; i < pwd_len; i++) {
        pass->values[i] = flash_read_byte(start_addr + i);
    }
    pass->is_set = 1;
}

void init_pwd_settings(void)
{
    uint32_t master_pwd_addr, pwd_addr;

    master_pwd_addr = _get_pwd_start_addr(FIRST_ADDR_MS_PWD, LAST_ADDR_MS_PWD,
                                            ADDR_MASTER_PASS_LEN);
    if(master_pwd_addr != NOT_AVAILABLE_ADDR)
        _load_pwd(master_pwd_addr, ADDR_MASTER_PASS_LEN, &master_password);

    pwd_addr = _get_pwd_start_addr(FIRST_ADDR_PWD, LAST_ADDR_PWD,
                                    ADDR_PASS_LEN);
    if(pwd_addr != NOT_AVAILABLE_ADDR)
        _load_pwd(pwd_addr, ADDR_PASS_LEN, &password);
}

char get_pass_symbol (uint8_t position, bool is_master)
{

    if (is_master) {
        if (!position && position >= MASTER_PASS_LEN)
            return 255;
        return (char) new_master_password.values[position];
    } else {
        if (!position && position >= PASS_LEN)
            return 255;
        return (char) new_password.values[position];
    }
}

uint8_t get_pass_len(void)
{
    return password.pass_len;
}

char inc_value(uint8_t position, bool is_master)
{
    if(is_master) {
        if (!position && position >= MASTER_PASS_LEN)
            return 255;
        if ('9' == new_master_password.values[position]) {
             new_master_password.values[position] = '0';
             return (char) new_master_password.values[position];
        }
        return (char) ++new_password.values[position];
    } else {
        if (!position && position >= PASS_LEN)
            return 255;
        if ('9' == new_password.values[position]) {
             new_password.values[position] = '0';
             return (char) new_password.values[position];
        }
        return (char) ++new_password.values[position];
    }
}

char dec_value(uint8_t position, bool is_master)
{
    if(is_master) {
        if (!position && position >= MASTER_PASS_LEN)
            return 255;
        if ('0' == new_master_password.values[position]) {
             new_master_password.values[position] = '9';
             return (char) new_master_password.values[position];
        }
        return (char) --new_password.values[position];
    } else {
        if (!position && position >= PASS_LEN)
            return 255;
        if ('0' == new_password.values[position]) {
             new_password.values[position] = '9';
             return (char) new_password.values[position];
        }
        return (char) --new_password.values[position];
    }
}

bool is_pwd_correct(bool is_master)
{
    if(is_master) {
        for(uint8_t i = 0; i < MASTER_PASS_LEN; i++) {
            if(new_master_password.values[i] != master_password.values[i])
                return false;
        }
        return true;
    } else {
        for(uint8_t i = 0; i < PASS_LEN; i++) {
            if(new_password.values[i] != password.values[i])
                return false;
        }
        return true;
    }
}
