#include <stdio.h>
#include <stdlib.h>
#include "../include/utils/commands.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"
static void verbose(command_t *self) {
  printf("verbose: enabled\n");
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
static void required(command_t *self) {
  printf("required: %s\n", self->arg);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
static void optional(command_t *self) {
  printf("optional: %s\n", self->arg);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
static void new_config_command(command_t *self) {
    printf("new_config_command called\n");
    if (self->argc > 0) {
      for (long unsigned int i = 0; i <= array_len(self->argv); i++) {
          printf("%s\n", self->argv[i]);
      }
    }
}

#pragma GCC diagnostic ignored "-Wpragmas"
// should save returned variable in command_object
command_object *setup_command_object(char *name, char *version) {
    command_object *cmdobj = (command_object *)malloc(sizeof(command_object));
    command_init(&cmdobj->cmd, name, version);
    command_option(&cmdobj->cmd, "new-config", "--ncfg", "create config file", new_config_command);
    command_option(&cmdobj->cmd, "-v", "--verbose", "enable verbose stuff", verbose);
    command_option(&cmdobj->cmd, "-r", "--required <arg>", "required arg", required);
    command_option(&cmdobj->cmd, "-o", "--optional [arg]", "optional arg", optional);
    return cmdobj;
}

int main(int argc, char **argv) {
    command_object *cmdobj = setup_command_object("test-command", "0.0.1");
    command_parse(&cmdobj->cmd, argc, argv);
    command_free(&cmdobj->cmd);
    return 0;
}