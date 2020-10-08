#include "password.h"
#include "spi.h"

#include <stdio.h>

static struct Password m_pwd = {
    .pass_len = MASTER_PASS_LEN,
    .val = {'0', '1', '0', '1'},
    .is_set = false
};

static struct Password pwd = {
    .pass_len = PASS_LEN,
    .val = {255, 255, 255, 255, 255, 255},
    .is_set = false
};


static struct Password new_m_pwd = {
    .pass_len = MASTER_PASS_LEN,
    .val = {'0', '0', '0', '0'},
    .is_set = false
};

static struct Password new_pwd = {
    .pass_len = PASS_LEN,
    .val = {'0', '0', '0', '0', '0', '0'},
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
        pass->val[i] = flash_read_byte(start_addr + i);
    }
    pass->is_set = 1;
}

void init_pwd_settings(void)
{
    uint32_t master_pwd_addr, pwd_addr;

    master_pwd_addr = _get_pwd_start_addr(FIRST_ADDR_MS_PWD, LAST_ADDR_MS_PWD,
                                            ADDR_MASTER_PASS_LEN);
    if(master_pwd_addr != NOT_AVAILABLE_ADDR)
        _load_pwd(master_pwd_addr, ADDR_MASTER_PASS_LEN, &m_pwd);

    pwd_addr = _get_pwd_start_addr(FIRST_ADDR_PWD, LAST_ADDR_PWD,
                                    ADDR_PASS_LEN);
    if(pwd_addr != NOT_AVAILABLE_ADDR)
        _load_pwd(pwd_addr, ADDR_PASS_LEN, &pwd);
}

char get_pass_symbol (uint8_t pos, bool is_master)
{
    uint8_t len = is_master ? MASTER_PASS_LEN : PASS_LEN;
    uint8_t *p = is_master ? new_m_pwd.val : new_pwd.val;

    if (pos >= len)
        return 255;

    return p[pos];
}

uint8_t get_pass_len(void)
{
    return pwd.pass_len;
}

char inc_value(uint8_t pos, bool is_master)
{
    uint8_t len = is_master ? MASTER_PASS_LEN : PASS_LEN;
    uint8_t *p = is_master ? new_m_pwd.val : new_pwd.val;

    if (pos >= len)
        return 255;

    p[pos]++;

    if (p[pos] > '9')
        p[pos] = '0';

    printf("p pos is  %u\n", p[pos]);
    return p[pos];
}

char dec_value(uint8_t pos, bool is_master)
{
    uint8_t len = is_master ? MASTER_PASS_LEN : PASS_LEN;
    uint8_t *p = is_master ? new_m_pwd.val : new_pwd.val;

    if (pos >= len)
        return 255;

    p[pos]--;

    if (p[pos] < '0')
        p[pos] = '9';

    return p[pos];
}

bool is_pwd_correct(bool is_master)
{
    if (is_master) {
        for (uint8_t i = 0; i < MASTER_PASS_LEN; i++) {
            if (new_m_pwd.val[i] != m_pwd.val[i])
                return false;
        }
        return true;
    } else {
        for (uint8_t i = 0; i < PASS_LEN; i++) {
            if (new_pwd.val[i] != pwd.val[i])
                return false;
        }
        return true;
    }
}
