#include <stdio.h>
#include "../../deps/commander/commander.h"
#include "array_len.h"

typedef struct command_object {
    command_t cmd;
} command_object;

// command to create a new config file
static void new_config_command(command_t *self);
static void verbose(command_t *self);
static void required(command_t *self);
static void optional(command_t *self);
// setups the commander package
command_object  *setup_command_object(char *name, char *version);