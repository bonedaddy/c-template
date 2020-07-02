/*
    
    attribution note: modified version of commander see `deps/commander` for the license for that package
    
    cli.h is a basic command line client header file
    You can extend it with additional commands according to your needs.
    Extending with a new command consists of two things:
      * Defining a callback function
      * Defining a function to return a command_handler of your new command
      * Update new_command_object to match the given input against your new command_handl;er
*/
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

// command_handler is an individual command
typedef struct {
  char *name;
  char *argv[MAX_COMMAND_ARGS];
  command_handler_callback callback;
} command_handler;

// command_object is an implementation of command
typedef struct command {
  int command_count;
  char *argv[MAX_COMMAND_ARGS];
  // make sure to free command_handler once you are done with it
  command_handler *command;
  command_handler commands[MAX_COMMANDS];
  int argc;
} command_object;

bool is_flag_argument(char *arg);
int load_command(command_object *pcobj, command_handler command);
// parses argc and argv to generate the root command object
command_object *new_command_object(int argc, char *argv[]);
// deallocs the memory associated with the given command_handler (required to be ran for no memory leaks)
int free_command_handler(command_object *pcobj);
// runs the command_handler and deallocs its memory
int run_and_free_command_handler(command_object *pcobj);
 