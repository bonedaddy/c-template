
/*! @file command_line.h
  * @brief provides a basic CLI building tool
  * attribution note: modified version of commander see `deps/commander` for the license for that package
  * to properly use you must load command handlers after constructing the command_object, and prepare the command line inputs for execution
  * this process consists of parsing `argc` and `argv` to determine which command to run, and prepares the input for execution
  * for a propery example see `cli.c`
*/

// pragma once is an alternative to header guards which can be very verbose
#pragma once

#include <stdbool.h>

#ifndef MAX_COMMANDS
#define MAX_COMMANDS 32
#endif

#ifndef MAX_COMMAND_ARGS
#define MAX_COMMAND_ARGS 32
#endif

// command is the main object type
struct command;

// declares the command handler callback which takes in an instance of command
typedef void (*command_handler_callback)(int argc, char *argv[]);

/*! @struct an individual command to run
  * @brief callback is a function to be executed
*/
typedef struct {
  char *name;
  command_handler_callback callback;
} command_handler;

/*! @struct the root command object
  * @brief contains the list of possible commands to execute
*/
typedef struct command {
  int command_count;
  int argc;
  char *argv[MAX_COMMAND_ARGS];
  command_handler *commands[MAX_COMMANDS];
} command_object;

/*! @brief determines whether or not the provided arg is a command instead of a flag
*/
bool is_command(char *arg);
/*! @brief checks whether or not the provided arg is a command line flag
  * We determine this by getting the first 2 char from the array and comparing that to `--`
*/
bool is_flag_argument(char *arg);

/*! @brief checks to see if we have a command named according to run and executes it
*/
int execute(command_object *self, char *command_to_run);

/*! @brief loads command handler and makes it executable
*/
int load_command(command_object *self, command_handler *command);

/*! @brief intializes a new command_object to have commands loaded into
*/
command_object *new_command_object(int argc, char *argv[]);

/*! @brief frees memory allocated for the command_object and sets pointer to null
*/
void free_command_object(command_object *self);