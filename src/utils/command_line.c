#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../../include/utils/command_line.h"
#include "../../include/utils/array_len.h"
#include "../../include/utils/logger.h"
#include "../../include/utils/safe_mem.h"


// checks whether or not the provide arg is a command line flag
// it does this by getting the first 2 char's and comparing that to `--`
#pragma GCC diagnostic ignored "-Wunused-function"
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

// returns a prepared command_object to execute user input
command_object *new_command_object(int argc, char *argv[]) {
  // if too many arguments have been provided return a null pointer
  if (argc > MAX_COMMAND_ARGS) {
    printf("too many command line arguments provided\n");
    return NULL;
  }
  // allocate memory equal to the size of command_object  combined with the size of all provided arguments
  // and typecast it to command_object *
  command_object *pcobj = (command_object *)malloc(sizeof(command_object) + sizeof(*argv));
  // set arg count
  pcobj->argc = argc;
  // set command handler as NULL
  pcobj->command = NULL;
  // parse cli arguments provided by user  and assign to pcobj
  for (int i = 0; i < argc; i++) {
    pcobj->argv[i] = argv[i];
  }
  return pcobj;
}

int load_command(command_object *pcobj, command_handler command) {
  if (pcobj->command_count >= MAX_COMMANDS) {
    printf("maximum number of commands");
    return -1;
  }
  int n = pcobj->command_count++;
  command_handler *cmd = &pcobj->commands[n];
  cmd->callback = command.callback;
  cmd->name = malloc(strlen(command.name) + 1);
  cmd->name = command.name;
  return 0;
}

// call this whenever you are done with a given command_handler
// generally done after it is invoked
int free_command_handler(command_object *pcobj) {
  if (pcobj->command == NULL) {
    printf("command_handler already freed\n");
    return -1;
  }
  free(pcobj->command);
  pcobj->command = NULL;
  return 0;
}

// helper function for running a command and freeing it
int run_and_free_command_handler(command_object *pcobj) {
  if(pcobj->command == NULL) {
    printf("command_handler is NULL\n");
    return -1;
  }
  pcobj->command->callback(pcobj->argc, pcobj->argv);
  return free_command_handler(pcobj);
}