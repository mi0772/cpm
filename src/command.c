#include "../include/command.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void read_command(char *command, int *state) {
    printf("> ");
    if (fgets(command, 256, stdin) != NULL) {
        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "exit") == 0) {
            *state = -1;
        } else {
            *state = 0;
        }
    } else {
        *state = -1;
    }
}

command_t *parse_command(char *command) {
    // token to take command and arguments
    char *word, *brkt;
    char *sep = " ";
    int counter = 0;
    command[strcspn(command, "\n")] = 0;

    command_t *result = calloc(1, sizeof(command_t));

    for (word = strtok_r(command, sep, &brkt); word; word = strtok_r(NULL, sep, &brkt)) {
        if (counter == 0) {
            result->command = parse_command_type(word);
            // se il comando non è riconosciuto esco senza analizzare eventuali parametri
            if (result->command == INVALID) return result;
        } else {
            result->args[counter - 1] = malloc(strlen(word) + 1);
            strcpy(result->args[counter - 1], word);
        }
        counter++;
    }
    result->argc = counter - 1;
    return result;
}

void free_command(command_t *command) {
    if (!command)
        return;

    for (int i = 0; i < command->argc; i++)
        free(command->args[i]);

    free(command);
}

command_type_e parse_command_type(char *command) {
    if (strcmp(command, "q") == 0) return QUIT;
    if (strcmp(command, "h") == 0) return HELP;
    if (strcmp(command, "a") == 0) return ADD;
    if (strcmp(command, "l") == 0) return LIST;
    if (strcmp(command, "f") == 0) return FIND;
    if (strcmp(command, "d") == 0) return DELETE;
    if (strcmp(command, "e") == 0) return EDIT;

    return INVALID;
}

char *command_type_to_string(command_type_e command_type) {
    switch (command_type) {
        case QUIT: return "QUIT";
        case HELP: return "HELP";
        case ADD: return "ADD";
        case LIST: return "LIST";
        case FIND: return "FIND";
        case DELETE: return "DELETE";
        case EDIT: return "EDIT";
        default: return "INVALID";
    }
}
