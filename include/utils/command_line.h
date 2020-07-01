/*
    
    attribution note: modified version of commander see `deps/commander` for the license for that package
    
    cli.h is a basic command line client header file
    You can extend it with additional commands according to your needs.
    Extending with a new command consists of two things:
      * Defining a callback function
      * Defining a function to return a command_handler of your new command
      * Update new_command_object to match the given input against your new command_handl;er
*/

#ifndef MAX_COMMAND_ARGS
#define MAX_COMMAND_ARGS 32
#endif

// command is the main object type
struct command;

// declares the command handler callback which takes in an instance of command
typedef void (* command_handler_callback)(int argc, char *argv[]);

// command_handler is an individual command
typedef struct {
  char *name;
  char *argv[MAX_COMMAND_ARGS];
  command_handler_callback callback;
} command_handler;

// command_object is an implementation of command
typedef struct command {
  char *argv[MAX_COMMAND_ARGS];
  command_handler *command;
  int argc;
} command_object;

// returns a prepared command_object to execute user input
command_object *new_command_object(int argc, char *argv[]);
// used to check if the provided string is a flag
bool is_flag_argument(char *arg);
