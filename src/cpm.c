#include "../include/command.h"
#include "../include/command_handler.h"
#include "../include/secure.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool logged_in = false;

static void set_master_password();
static void secure_init();
static secure_result login();

int main(void) {

    void display_result(command_result_t r);

    printf("Welcome to cpm 1.0 — password manager CLI\nType 'h' for help.\n\n");

    secure_init();

    char *command = malloc(256);
    int state = 0;

    while (1) {
        read_command(command, &state);
        command_t *cmd = parse_command(command);
        if (cmd->command == QUIT) {
            free_command(cmd);
            return 0;
        }
        command_result_t r = handle_command(cmd);
        display_result(r);

        free_command(cmd);
    }
}

static void set_master_password() {
    char password1[MASTER_PASSWORD_MAX_LEN];
    char password2[MASTER_PASSWORD_MAX_LEN];
    bool ok = false;

    do {
        printf("set master password : ");
        if (fgets(password1, sizeof(password1), stdin) != NULL) {
            password1[strcspn(password1, "\n")] = '\0';
        }
        printf("\nconfirm master password : ");
        if (fgets(password2, sizeof(password2), stdin) != NULL) {
            password2[strcspn(password2, "\n")] = '\0';
        }

        if (strcmp(password1, password2) != 0) {
            printf("password does not match\n");
        } else {
            secure_result r = secure_master_password_set(password1);
            if (r == SECURE_OK) {
                printf("master password set successfully\n");
                ok = true;
            } else {
                printf("error during storing master password, exit\n");
                return;
            }
        }
    } while (!ok);
}

static void secure_init() {
    if (!secure_master_password_is_set()) {
        set_master_password();
    } else {
        secure_result r;
        do {
            r = login();
        } while (r != SECURE_OK);
        logged_in = true;
    }
}

static secure_result login() {
    char password[MASTER_PASSWORD_MAX_LEN];
    printf("Enter master password: ");
    if (fgets(password, sizeof(password), stdin) != NULL) {
        password[strcspn(password, "\n")] = '\0';
    }
    return secure_master_password_verify(password);
}

void display_result(command_result_t r) {
    if (r == COMMAND_OK)
        printf("OK\n");
    if (r == COMMAND_ERROR)
        printf("ERROR\n");
    if (r == COMMAND_ERROR_KEY_EXIST)
        printf("ERROR: Already exists, use e to edit\n");
}
