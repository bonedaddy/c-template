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
  // parse cli arguments provided by user  and assign to pcobj
  for (int i = 0; i < argc; i++) {
    pcobj->argv[i] = argv[i];
    pcobj->argc = argc;
  }
  return pcobj;
}
