#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <argtable3.h>
#include "../../include/utils/command_line.h"
#include "../../include/utils/array_len.h"
#include "../../include/utils/logger.h"
#include "../../include/utils/safe_mem.h"

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
    pcobj->argv[i] = malloc(strlen(argv[i]) + 1);
    strcpy(pcobj->argv[i], argv[i]);
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

// validates argtable and parses command line arguments
int parse_args(int argc, char *argv[], void *argtable[]) {
    if (arg_nullcheck(argtable) != 0) {
        return -1;
    }
    int nerrors = arg_parse(argc, argv, argtable);
    // handle help before errors
    if (help->count > 0) {
        print_help(argv[0], argtable);
        return -2;
    }
    if (nerrors > 0) {
        arg_print_errors(stdout, end, "main");
        return -1;
    }
    return 0;
}

void print_help(char *program_name, void *argtable[]) {
    printf("Usage: %s", program_name);
    arg_print_syntax(stdout, argtable, "\n");
    arg_print_glossary(stdout, argtable, " %-25s %s\n");
}

// sets up default arguments, you will have to do this for user defined args
void setup_args(const char *version_string) {
    help = arg_litn(NULL, "help", 0, 1, "displays help menu");
    version = arg_litn(NULL, "version", 0, 1, version_string);
    file = arg_filen(NULL, "file", "<file>", 0, 10, "file(s) to load data from max of 10");
    output = arg_filen(NULL, "output", "<file>", 0, 1, "file to output data too");
    command_to_run = arg_strn("c", "command", "<command>", 0, 1, "command to run");
    end = arg_end(20);
} 

char *get_run_command() {
  char *run_command = malloc(strlen(*command_to_run->sval));
  strcpy(run_command, (char *)*command_to_run->sval);
  return run_command;
}