//
// Created by Carlo Di Giuseppe on 30/05/25.
//
#include <stdbool.h>

#ifndef SECURE_H
#define SECURE_H

#define MASTER_PASSWORD_MAX_LEN 32

typedef enum {
    SECURE_OK,
    SECURE_PASSWORD_INVALID,
    SECURE_ERROR,
} secure_result;

bool secure_master_password_is_set();
secure_result secure_master_password_set(const char *master_password);
secure_result secure_master_password_verify(char *user_entry_password);

#endif // SECURE_H
