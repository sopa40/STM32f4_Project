#include "password.h"

//TODO: refactor = is_master ? :  with arrays

#include <stdio.h>

static struct Password m_pwd = {
    .pwd_len = MASTER_PASS_LEN,
    .val = {255, 255, 255, 255},
    .attempts = 0,
    .is_set = false,
    .is_init = false
};

static struct Password pwd = {
    .pwd_len = PASS_LEN,
    .val = {255, 255, 255, 255, 255, 255},
    .attempts = 0,
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

static uint8_t plen [2] = {PASS_LEN, MASTER_PASS_LEN};
static uint8_t *new_pptr [2] = {new_pwd.val, new_m_pwd.val};
static struct Password *str_ptr [2] = {&pwd, &m_pwd};
static uint32_t is_pwd_set_s [2] = {FIRST_ADDR_PWD_IS_SET, FIRST_ADDR_MS_PWD_IS_SET};
static uint32_t is_pwd_set_e [2] = {LAST_ADDR_PWD_IS_SET, LAST_ADDR_MS_PWD_IS_SET};
static uint32_t pwd_val_s [2] = {FIRST_ADDR_PWD, FIRST_ADDR_MS_PWD};
static uint32_t pwd_val_e [2] = {LAST_ADDR_PWD, LAST_ADDR_MS_PWD};
static uint32_t attempts_s [2] = {FIRST_ADDR_ATTEMPTS, FIRST_ADDR_M_ATTEMPTS};
static uint32_t attempts_e [2] = {LAST_ADDR_ATTEMPTS, LAST_ADDR_M_ATTEMPTS};
static uint32_t delay_s [2] = {FIRST_ADDR_DELAY, FIRST_ADDR_M_DELAY};
static uint32_t delay_e [2] = {LAST_ADDR_DELAY, LAST_ADDR_M_DELAY};


bool is_init(bool is_master)
{
    if (is_master)
        return m_pwd.is_init;
    else
        return pwd.is_init;

    return false;
}

void clear_pwd_input(bool is_master)
{
    uint8_t len = plen[is_master];
    uint8_t *p = new_pptr[is_master];
    for (uint8_t i = 0; i < len; i++)
        p[i] = '0';
}

static void _load_default_m_pwd(void)
{
    for (uint8_t i = 0; i < MASTER_PASS_LEN; i++) {
        m_pwd.val[i] = flash_read_byte(FIRST_ADDR_DEFAULT_M_PWD + i);
        if (m_pwd.val[i] == 255) {
            //TODO: handle it as a fatal error
            print_error("FATAL ERROR!\n");
        }
    }
    m_pwd.is_init = 1;
}

static void _load_pwd(bool is_master)
{
    uint8_t len = plen[is_master];
    struct Password *p = str_ptr[is_master];
    uint32_t start_addr = pwd_val_s[is_master];
    uint32_t end_addr = pwd_val_e[is_master];
    uint32_t pwd_start = find_free_addr(start_addr, end_addr) - len;
    if (pwd_start < start_addr || pwd_start + len == NOT_AVAILABLE_ADDR) {
        print_error("fatal error");
    } else {
        for (uint8_t i = 0; i < len; i++) {
            p->val[i] = flash_read_byte(pwd_start + i);
        }
        p->is_init = 1;
    }
}

//TODO: if is set - 0; not 0 and not 255 - corrupted memory
static bool _is_pwd_set(bool is_master)
{
    uint32_t start_addr = is_pwd_set_s[is_master];
    uint32_t end_addr = is_pwd_set_e[is_master];
    uint32_t free_addr = find_free_addr(start_addr, end_addr);
    if (free_addr == NOT_AVAILABLE_ADDR || free_addr == start_addr)
        return false;
    return true;
}

//TODO: fix bug with last addr, it wont be overwritten. Just error case
void _set_attempts(bool is_master)
{
    struct Password *p = str_ptr[is_master];
    uint32_t start_addr = attempts_s[is_master];
    uint32_t end_addr = attempts_e[is_master];
    uint32_t addr = find_free_addr(start_addr, end_addr);
    if (addr == NOT_AVAILABLE_ADDR)
        print_error("memory fault att");

    if (addr == start_addr)
        p->attempts = 3;
    else
        p->attempts = flash_read_byte(addr - 1);

    printf("attempts are %u, in %u\n", p->attempts, is_master);
    if(!p->attempts)
        wait_to_try(is_master);
}

bool init_pwd_settings(void)
{
    if (!is_init(true)) {
        if (_is_pwd_set(true))
            _load_pwd(true);
        else
            _load_default_m_pwd();
        _set_attempts(true);
    }

    if (!is_init(false)) {
        if (_is_pwd_set(false)) {
            printf("pwd is set\n");
            _load_pwd(false);
        }
        else {
            if(m_pwd.val[0] != 255)
                print_no_pwd_msg();
            return false;
        }
        _set_attempts(false);
    }
    return true;
}

char get_pwd_sym(uint8_t pos, bool is_master)
{
    uint8_t len = plen[is_master];
    uint8_t *p = new_pptr[is_master];

    if (pos >= len)
        return 255;
    return p[pos];
}

uint8_t get_pwd_len(bool is_master)
{
    if (is_master)
        return m_pwd.pwd_len;
    else
        return pwd.pwd_len;
}

static char handle_pos_move(uint8_t pos, bool is_master, bool is_inc)
{
	uint8_t len = plen[is_master];
    uint8_t *p = new_pptr[is_master];

	uint8_t diff = is_inc ? 10 : (uint8_t)(-10);
	uint8_t inc = is_inc ? 1 : (uint8_t)(-1);

    if (pos >= len)
        return 255;

    p[pos] += inc;

	if (p[pos] > '9' || p[pos] < '0')
		p[pos] -= diff;

	return p[pos];
}

char inc_value(uint8_t pos, bool is_master) {
	return handle_pos_move(pos, is_master, true);
}

char dec_value(uint8_t pos, bool is_master) {
	return handle_pos_move(pos, is_master, false);
}

bool is_pwd_eq(bool is_master)
{

    uint8_t len = plen[is_master];
    struct Password *p = str_ptr[is_master];
    uint8_t *new_p = new_pptr[is_master];

    if (!p->is_init) {
        print_error("pwd not init. Fatal");
        return false;
    }

    for (uint8_t i = 0; i < len; i++) {
        if (p->val[i] != new_p[i])
            return false;
    }

    return true;
}

void save_pwd(bool is_master)
{
    uint8_t len = plen[is_master];
    struct Password *p = str_ptr[is_master];
    uint32_t start_addr = pwd_val_s[is_master];
    uint32_t end_addr = pwd_val_e[is_master];
    uint32_t is_set_addr_s = is_pwd_set_s[is_master];
    uint32_t is_set_addr_e = is_pwd_set_e[is_master];

    uint32_t addr = find_free_addr(start_addr, end_addr);
    uint32_t is_set_addr = find_free_addr(is_set_addr_s, is_set_addr_e);
    if (addr == NOT_AVAILABLE_ADDR || addr + len > end_addr) {
        clear_page(start_addr, start_addr);
        addr = find_free_addr(start_addr, end_addr);
    }
    if (is_set_addr == NOT_AVAILABLE_ADDR) {
        clear_page(is_set_addr_s, is_set_addr_e);
        is_set_addr = find_free_addr(is_set_addr_s, is_set_addr_e);
    }

    flash_unlock();
    for (uint32_t i = 0; i < len; i++) {
        flash_write_byte(addr + i, p->val[i]);
    }
    flash_write_byte(is_set_addr, 0x000000);
    flash_lock();
    _load_pwd(false);
}

void dec_attempts(bool is_master)
{
    struct Password *p = str_ptr[is_master];
    uint32_t start_addr = attempts_s[is_master];
    uint32_t end_addr = attempts_e[is_master];
    uint32_t addr = find_free_addr(start_addr, end_addr);
    if (p->attempts <= 3 && p->attempts > 0) {
        p->attempts--;
        if (addr == NOT_AVAILABLE_ADDR) {
            clear_page(start_addr, end_addr);
            addr = find_free_addr(start_addr, end_addr);
        }
        flash_write_byte(addr, p->attempts);
    }
    else
        print_error("-1 attepmts! Error");
}

void restore_attempts(bool is_master)
{
    struct Password *p = str_ptr[is_master];
    uint32_t att_addr_s = attempts_s[is_master];
    uint32_t att_addr_e = attempts_e[is_master];
    uint32_t delay_addr_s = delay_s[is_master];
    uint32_t delay_addr_e = delay_e[is_master];
    uint32_t att_addr = find_free_addr(att_addr_s, att_addr_e);
    uint32_t delay_addr = find_free_addr(delay_addr_s, delay_addr_e);

    p->attempts = 3;

    if (att_addr == NOT_AVAILABLE_ADDR) {
        clear_page(att_addr_s, att_addr_e);
        att_addr = find_free_addr(att_addr_s, att_addr_e);
    }

    if (delay_addr == NOT_AVAILABLE_ADDR) {
        clear_page(delay_addr_s, delay_addr_e);
        delay_addr = find_free_addr(delay_addr_s, delay_addr_e);
    }

    flash_write_byte(att_addr, p->attempts);
    flash_write_byte(delay_addr, 0);
}

void add_attempt(bool is_master)
{
    struct Password *p = str_ptr[is_master];
    uint32_t start_addr = attempts_s[is_master];
    uint32_t end_addr = attempts_e[is_master];
    uint32_t addr = find_free_addr(start_addr, end_addr);
    p->attempts = 1;
    if (addr == NOT_AVAILABLE_ADDR) {
        clear_page(start_addr, end_addr);
        addr = find_free_addr(start_addr, end_addr);
    }
    flash_write_byte(addr, p->attempts);
}

uint8_t get_attempts(bool is_master)
{
    if (is_master)
        return m_pwd.attempts;
    else
        return pwd.attempts;
}

void wait_to_try(bool is_master)
{
    uint32_t wait_time = 1;
    uint32_t start_addr = delay_s[is_master];
    uint32_t end_addr = delay_e[is_master];
    uint32_t addr = find_free_addr(start_addr, end_addr);
    uint8_t fac_val;
    if (addr == NOT_AVAILABLE_ADDR)
        print_error("memory err wait");
    if (addr == start_addr)
        fac_val = 0;
    else
        fac_val = flash_read_byte(addr - 1);

    if (fac_val == 255)
        print_error("invalid wait time");

    for (uint8_t i = 0; i < fac_val; i++) {
        wait_time *= DELAY_FAC;
    }

    print_info("Enter pass in");
    print_error(" ");
    for (uint32_t i = wait_time; i > 0; i--) {
        print_wait_time(i);
        sk_tick_delay_ms(1000);
    }
    flash_write_byte(addr, fac_val + 1);
    add_attempt(is_master);
    clear_pwd_input(is_master);
    draw_pwd_input(is_master);
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
