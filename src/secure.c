//
// Created by Carlo Di Giuseppe on 30/05/25.
//

#include "../include/secure.h"
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MASTER_PASSWORD_FILE "cpm.mpwd"

bool secure_master_password_is_set() {
    FILE *f = fopen(MASTER_PASSWORD_FILE, "r");
    if (!f) {
        return false;
    }
    char p[MASTER_PASSWORD_MAX_LEN];
    if (fgets(p, sizeof(p), f) != NULL) {
        fclose(f);
        return true;
    }
    fclose(f);
    return false;
}

secure_result secure_master_password_set(const char *master_password) {
    FILE *f = fopen(MASTER_PASSWORD_FILE, "w");
    if (!f) {
        return SECURE_ERROR;
    }

    size_t len = strnlen(master_password, MASTER_PASSWORD_MAX_LEN);
    if (len == MASTER_PASSWORD_MAX_LEN) {
        // Password troppo lunga, non accettabile
        fclose(f);
        return SECURE_ERROR;
    }

    if (fwrite(master_password, 1, len, f) != len) {
        fclose(f);
        return SECURE_ERROR;
    }

    fclose(f);
    return SECURE_OK;
}

secure_result secure_master_password_verify(char *user_entry_password) {
    FILE *f = fopen(MASTER_PASSWORD_FILE, "r");
    if (!f) {
        return SECURE_ERROR;
    }

    char master_password[MASTER_PASSWORD_MAX_LEN + 1] = {0};
    size_t read_bytes = fread(master_password, 1, MASTER_PASSWORD_MAX_LEN, f);
    fclose(f);
    if (read_bytes == 0) {
        return SECURE_ERROR;
    }

    // Rimuove newline se presente
    master_password[strcspn(master_password, "\n")] = 0;

    if (strncmp(user_entry_password, master_password, MASTER_PASSWORD_MAX_LEN) != 0) {
        return SECURE_PASSWORD_INVALID;
    }

    return SECURE_OK;
}
