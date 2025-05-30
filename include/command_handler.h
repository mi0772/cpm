//
// Created by Carlo Di Giuseppe on 30/05/25.
//

#ifndef COMMAND_HANDLER_H
#define COMMAND_HANDLER_H
#include "command.h"

typedef enum {
    COMMAND_OK,
    COMMAND_ERROR,
    COMMAND_ERROR_KEY_EXIST,
    COMMAND_ERROR_WRONG_ARGUMENT,
    COMMAND_ERROR_KEY_NOT_FOUND,
} command_result_t;

command_result_t handle_command(command_t *cmd);

#endif //COMMAND_HANDLER_H
