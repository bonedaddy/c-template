#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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
static void new_zlog_config_command(command_t *self) {
    char *configPath;
    // if we havve more than 0 arguments check for `--config` or `--cfg`
    if (self->argc > 0) {
      for (long unsigned int i = 0; i <= array_len(self->argv); i++) {
        // if --config or --cfg is provided, then use the value of that as the zlog config file location
        if (strcmp(self->argv[i], "--config") == 0  || strcmp(self->argv[i], "--cfg") == 0) {
          printf("found arg: %s\n", self->argv[i]);
          configPath = (char *)malloc(sizeof(configPath));
          strcpy(configPath, self->argv[i]);
        }
      }
    }
    // if configPath variable is empty, it means no valid config path was provided
    // thus we will default to zlog.conf
    if (strcmp(configPath, "") == 0) {
      configPath = "zlog.conf";
    }
    printf("%s\n", configPath);
}

#pragma GCC diagnostic ignored "-Wpragmas"
// should save returned variable in command_object
command_object *setup_command_object(char *name, char *version) {
    command_object *cmdobj = (command_object *)malloc(sizeof(command_object));
    command_init(&cmdobj->cmd, name, version);
    command_option(&cmdobj->cmd, "nlcfg", "new-logger-config", "create zlog configuration file", new_zlog_config_command);
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