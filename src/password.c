#include "password.h"

#include <stdio.h>


static struct Password password = {
    .pass_len = PASS_LEN,
    .values = {'1', '2', '3', '4', '5', '6'},
    .is_set = false
};

static struct Password new_password = {
    .pass_len = PASS_LEN,
    .values = {'0', '0', '0', '0', '0', '0'},
    .is_set = false
};

char get_pass_symbol (uint8_t position)
{
    if (!position && position >= PASS_LEN)
        return 255;
    return (char) new_password.values[position];
}

uint8_t get_pass_len(void)
{
    return password.pass_len;
}

char inc_value(uint8_t position)
{
    if (!position && position >= PASS_LEN)
        return 255;
    if ('9' == new_password.values[position]) {
         new_password.values[position] = '0';
         return (char) new_password.values[position];
    }
    return (char) ++new_password.values[position];
}

char dec_value(uint8_t position)
{
    if (!position && position >= PASS_LEN)
        return 255;
    if ('0' == new_password.values[position]) {
         new_password.values[position] = '9';
         return (char) new_password.values[position];
    }
    return (char) --new_password.values[position];
}

bool is_pwd_correct(void)
{
    for(uint8_t i = 0; i < PASS_LEN; i++) {
        if(password.values[i] != password.values[i])
            return false;
    }
    return true;
}
