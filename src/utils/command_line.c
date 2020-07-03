#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../../include/utils/command_line.h"
#include "../../include/utils/array_len.h"
#include "../../include/utils/logger.h"
#include "../../include/utils/safe_mem.h"


/*! @brief checks whether or not the provided arg is a command line flag
  * We determine this by getting the first 2 char from the array and comparing that to `--`
*/
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

/*! @brief intializes a new command_object to have commands loaded into
*/
command_object *new_command_object(int argc, char *argv[]) {
  // if too many arguments have been provided return a null pointer
  if (argc > MAX_COMMAND_ARGS) {
    printf("too many command line arguments provided\n");
    return NULL;
  }
  // allocate memory equal to the size of command_object  combined with the size of all provided arguments
  // and typecast it to command_object *
  // command_object *pcobj = (command_object *)malloc(sizeof(command_object) + sizeof(*argv));
  // this prevents valgrind from reporting an error
  command_object *pcobj = calloc(sizeof(command_object), sizeof(*argv));
  // set arg count
  pcobj->argc = argc;
  // set command_count
  pcobj->command_count = 0;
  // parse cli arguments provided by user  and assign to pcobj
  for (int i = 0; i < argc; i++) {
    pcobj->argv[i] = argv[i];
  }
  return pcobj;
}

/*! @brief loads command handler and makes it executable
*/
int load_command(command_object *self, command_handler *command) {
  if (self->command_count >= MAX_COMMANDS) {
    printf("maximum number of commands\n");
    return -1;
  }
  int n = self->command_count++;
  self->commands[n] = command;
  return 0;
}

/*! @brief checks to see if we have a command named according to run and executes it
*/
int execute(command_object *self, char *run) {
  for (int i = 0; i < self->command_count; i++) {
    if (strcmp(self->commands[i]->name, run) == 0) {
      self->commands[i]->callback(self->argc, self->argv);
      return 0;
    }
  }
  printf("failed to execute command\n");
  return -1;
}

/*! @brief frees memory allocated for the command_object and sets pointer to null
   * for some reason this is causing an address boundary error 
*/
void free_command_object(command_object *self) {
  for (int i = 0; i < self->command_count; i++) {
      free(self->commands[i]);
  }
  free(self);
}