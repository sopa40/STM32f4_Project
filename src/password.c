#include "password.h"
#include "spi.h"
#include "lcd_menu.h"

#include <stdio.h>

static struct Password m_pwd = {
    .pwd_len = MASTER_PASS_LEN,
    .val = {255, 255, 255, 255},
    .is_set = false,
    .is_init = false
};

static struct Password pwd = {
    .pwd_len = PASS_LEN,
    .val = {255, 255, 255, 255, 255, 255},
    .is_set = false,
    .is_init = false
};

static struct New_Password new_m_pwd = {
    .pwd_len = MASTER_PASS_LEN,
    .val = {'0', '0', '0', '0'}
};

static struct New_Password new_pwd = {
    .pwd_len = PASS_LEN,
    .val = {'0', '0', '0', '0', '0', '0'}
};

bool is_init(bool is_master)
{
    if (is_master)
        return m_pwd.is_init;
    else
        return pwd.is_init;

    return false;
}

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

static void _load_default_m_pwd(void)
{
    uint32_t start_addr = FIRST_ADDR_DEFAULT_M_PWD;
    for (uint8_t i = 0; i < MASTER_PASS_LEN; i++) {
        m_pwd.val[i] = flash_read_byte(start_addr + i);
        if (m_pwd.val[i] == 255) {
            printf("fuck\n");
            print_error("FATAL ERROR!\n");
        }
    }
    m_pwd.is_init = 1;
}

static void _load_pwd(bool is_master)
{
    printf("in _load_pwd\n");
    uint8_t len = is_master ? MASTER_PASS_LEN : PASS_LEN;
    struct Password *p = is_master ? &m_pwd : &pwd;
    uint32_t start_addr = is_master ? FIRST_ADDR_MS_PWD : FIRST_ADDR_PWD;
    uint32_t end_addr = is_master ? LAST_ADDR_MS_PWD : LAST_ADDR_PWD;
    uint32_t pwd_start = find_free_addr(start_addr, end_addr) - len;
    printf("pwd start is %lu\n", pwd_start);
    if (pwd_start < start_addr || pwd_start + len == NOT_AVAILABLE_ADDR) {
        printf("FUCK");
        print_error("fatal error");
    }
    for (uint8_t i = 0; i < len; i++) {
        p->val[i] = flash_read_byte(pwd_start + i);
    }
    p->is_init = 1;
}

static bool _is_pwd_set(bool is_master)
{
    uint8_t len = is_master ? MASTER_PASS_LEN : PASS_LEN;
    uint32_t start_addr = is_master ? FIRST_ADDR_MS_PWD_IS_SET : FIRST_ADDR_PWD_IS_SET;
    uint32_t end_addr = is_master ? LAST_ADDR_MS_PWD_IS_SET : LAST_ADDR_PWD_IS_SET;
    uint32_t free_addr = find_free_addr(start_addr, end_addr);
    printf("free addr is %lu\n", free_addr);
    if (free_addr == NOT_AVAILABLE_ADDR || free_addr == start_addr)
        return false;
    return true;
}

void init_pwd_settings(void)
{
    if (!is_init(true)) {
        if (_is_pwd_set(true))
            _load_pwd(true);
        else
            _load_default_m_pwd();
    }

    if (!is_init(false)) {
        if (_is_pwd_set(false)) {
            printf("pwd is set\n");
            _load_pwd(false);
        }
        else
            printf("pwd is not set!\n");
    }
}

char get_pwd_sym(uint8_t pos, bool is_master)
{
    uint8_t len = is_master ? MASTER_PASS_LEN : PASS_LEN;
    uint8_t *p = is_master ? new_m_pwd.val : new_pwd.val;

    if (pos >= len)
        return 255;

    return p[pos];
}

uint8_t get_pwd_len(void)
{
    return pwd.pwd_len;
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
    uint8_t len = is_master ? MASTER_PASS_LEN : PASS_LEN;
    uint8_t *p = is_master ? m_pwd.val : pwd.val;
    uint8_t *new_p = is_master ? new_m_pwd.val : new_pwd.val;

    for (uint8_t i = 0; i < len; i++) {
        if (p[i] != new_p[i])
            return false;
    }

    return true;
}

void get_m_pwd(void)
{
    for (int i = 0; i < MASTER_PASS_LEN; i++) {
        printf("%u\n", m_pwd.val[i]);
    }
}

void get_pwd(void)
{
    for (int i = 0; i < PASS_LEN; i++) {
        printf("%u\n", pwd.val[i]);
    }
}
