/*
 * Author: Carlo Di Giuseppe <cdigiuseppe@me.com>
 * Created: 2025-05-29
 * Description: [Breve descrizione del contenuto o scopo del file]
 *
 * License: MIT License
 *          Permission is hereby granted, free of charge, to any person obtaining a copy
 *          of this software and associated documentation files (the "Software"), to deal
 *          in the Software without restriction, including without limitation the rights
 *          to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *          copies of the Software, and to permit persons to whom the Software is
 *          furnished to do so, subject to the following conditions:
 *          [.. puoi accorciare qui o mettere il testo completo in un LICENSE file ..]
 */

#ifndef COMMAND_PARSER
#define COMMAND_PARSER

#define MAX_ARGS 10
typedef enum {
    ADD = 1,
    LIST = 2,
    FIND = 3,
    DELETE = 4,
    EDIT = 5,
    HELP = 80,
    QUIT = 90,
    INVALID = 99
} command_type_e;

typedef struct {
    command_type_e command;
    char *args[MAX_ARGS];
    int argc;
    int state;
} command_t;



void read_command(char *command, int *state);
command_t *parse_command(char *command);
void free_command(command_t *command);
command_type_e parse_command_type(char *command);
char *command_type_to_string(command_type_e command_type);

#endif
