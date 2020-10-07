#include "password.h"
#include <stdbool.h>

//TODO: add pass len setting in master menu
#define PASS_LEN 6

static struct Password password = {
    .pass_len = PASS_LEN,
    .values[PASS_LEN] = {0},
    .is_set = false;
}

struct Password *get_password(void)
{
		return &password;
}
