//
// Created by Carlo Di Giuseppe on 30/05/25.
//
#include "../include/command.h"
#include "../include/command_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/vault.h"

static command_result_t display_usage(void);
static command_result_t do_help(void);
static command_result_t do_add(void);
static command_result_t do_find(const command_t *cmd);
static command_result_t do_list();

command_result_t handle_command(command_t *cmd) {
    switch (cmd->command) {
        case QUIT: break;
        case INVALID:
            printf("Invalid command\n");
            return COMMAND_OK;
        case HELP:
            return do_help();
        case ADD:
            return do_add();
        case EDIT:
            break;
        case DELETE:
            break;
        case LIST:
            do_list();
            break;
        case FIND:
            do_find(cmd);
            break;
        default:
            return do_help();
    }
}

static command_result_t do_help() {
    display_usage();
    return COMMAND_OK;
}

static void strip_newline(char *s);

static command_result_t do_find(const command_t *cmd) {
    if (cmd->argc < 1) {
        printf("Not enough arguments\n");
        return COMMAND_ERROR_WRONG_ARGUMENT;
    }
    uint32_t hash = calc_hash(cmd->args[0]);
    if (!vault_entry_exist(cmd->args[0])) {
        return COMMAND_ERROR_KEY_NOT_FOUND;
    }

    vault_entry *entry = vault_entry_create();
    if (get_from_vault(hash, entry) == OK) {
        printf("label: %s\n", entry->key);
        printf("UserID/Email: %s\n", entry->identifier);
        printf("Password: %s\n", entry->password);
        free(entry);
        return COMMAND_OK;
    }
    return COMMAND_ERROR;
}

static command_result_t do_list() {
    vault_entry *entries = NULL;
    size_t count = 0;

    if (get_all_from_vault(&entries, &count) == OK) {
        printf("+----+----------------------+----------------------+----------------------+\n");
        printf("| #  | Label                | UserID/Email         | Password             |\n");
        printf("+----+----------------------+----------------------+----------------------+\n");

        for (size_t i = 0; i < count; i++) {
            printf("| %-2zu | %-20s | %-20s | %-20s |\n",
                i,
                entries[i].key,
                entries[i].identifier,
                entries[i].password
            );
        }

        printf("+----+----------------------+----------------------+----------------------+\n");
        free(entries);
    } else {
        printf("Vault empty or error reading entries.\n");
        return COMMAND_ERROR;
    }

    return COMMAND_OK;
}

static command_result_t do_add() {

    vault_entry *new_entry = vault_entry_create();

    printf("Label : ");
    if (fgets(new_entry->key, 256, stdin) == NULL) return COMMAND_ERROR;
    strip_newline(new_entry->key);

    if (vault_entry_exist(new_entry->key)) {
        return COMMAND_ERROR_KEY_EXIST;
    }

    printf("UserId or Email : ");
    if (fgets(new_entry->identifier, 256, stdin) == NULL) return COMMAND_ERROR;
    strip_newline(new_entry->identifier);

    printf("Password : ");
    if (fgets(new_entry->password, 256, stdin) == NULL) return COMMAND_ERROR;
    strip_newline(new_entry->password);

    file_operation_result_t r = put_to_vault(new_entry);
    vault_entry_destroy(new_entry);

    if (r != OK)
        return COMMAND_ERROR;

    return COMMAND_OK;
}

static command_result_t display_usage(void) {
    printf("\nUsage:\n");
    printf("  cpm [command] [arguments]\n\n");
    printf("Commands:\n");
    printf("  a            add a new entry\n");
    printf("  l            list all entries\n");
    printf("  f <label>    search for entry with given label\n");
    printf("  d <label>    delete entry with given label\n");
    printf("  e <label>    edit entry with given label\n");
    printf("  h            show this help message\n");
    printf("  q            quit the program\n\n");
    return COMMAND_OK;
}

static void strip_newline(char *s) {
    size_t len = strlen(s);
    if (len > 0 && s[len - 1] == '\n') s[len - 1] = '\0';
}