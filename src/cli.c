#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <argtable3.h>
#include <string.h>
#include "../include/utils/array_len.h"
#include "../include/utils/logger.h"

#ifndef MAX_COMMAND_ARGS
#define MAX_COMMAND_ARGS 32
#endif


struct command;

typedef void (* command_handler_callback)(struct command *self);

// a callback function
typedef struct {
  char *name;
  //void (*callback)(char *argv[])();
  command_handler_callback callback;
} command_handler;

typedef struct command {
  char *argv[MAX_COMMAND_ARGS];
  command_handler *commands[MAX_COMMAND_ARGS];
  int argc;
} command_object;

// used to check if the provided string is a flag
bool is_flag_argument(char *arg);
// returns a prepared command_object to execute user input
command_object *new_command_object();
// a command to generate a new zlog configuration
command_handler *new_zlog_config_command();
void new_logger_config_wrapper();

void new_logger_config_wrapper() {
  printf("new_logger_config_wrapper called\n");
}

#pragma GCC diagnostic ignored "-Wunused-function"
command_handler *new_zlog_config_command() {
  // allocate size of command_handler and the new-log-config `char *`
  command_handler *handler = malloc(sizeof(command_handler) + sizeof("new-log-config"));
  handler->callback = new_logger_config_wrapper;
  handler->name = "new-log-config";
  return handler;
}
// checks whether or not the provide arg is a command line flag
// it does this by getting the first 2 char's and comparing that to `--`
bool is_flag_argument(char *arg) {
    if (strlen(arg) > 2) {
      char c[2] = {arg[0], arg[1]};
      char *part = c;
      if (strcmp(part, "--") == 0) {
        return true;
      }
    }
  return false;
}

// parses argc and argv to generate the root command object
command_object *new_command_object(int argc, char *argv[]) {
  // if too many arguments have been provided return a null pointer
  if (argc > MAX_COMMAND_ARGS) {
    printf("too many command line arguments provided\n");
    return NULL;
  }
  // allocate memory equal to the size of command_object  combined with the size of all provided arguments
  // and typecast it to command_object *
  command_object *pcobj = (command_object *)malloc(sizeof(command_object) + sizeof(*argv));
  for (int i = 0; i < argc; i++) {
    // we only accept whole commands or flag with `--argName`
    if (strlen(argv[i]) >= 2) {
      // check to see if arg is a command line flag
      if (is_flag_argument(argv[i])) {
        // TODO(bonedaddy): handle
        printf("found flag: %s\n", argv[i]);
      }
    }
    pcobj->argv[i] = argv[i];
  }
  return pcobj;
}

int main(int argc, char *argv[]) {
  command_handler *cmd = new_zlog_config_command();
  if (cmd != NULL) {
    cmd->callback();
  }
  command_object *pcmd = new_command_object(argc, argv);
  if (pcmd == NULL) {
    printf("failed to get command_object");
    return - 1;
  }
  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      printf("%s\n", pcmd->argv[i]);
    }
  }
}