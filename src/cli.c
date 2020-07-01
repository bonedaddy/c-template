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

/*
  command is the main object type
*/
struct command;

/*
  declares the command handler callback which takes in an instance of command
*/
typedef void (* command_handler_callback)(struct command *self);

/*
  command_handler is an individual command
*/
typedef struct {
  char *name;
  char *argv[MAX_COMMAND_ARGS];
  //void (*callback)(char *argv[])();
  command_handler_callback callback;
} command_handler;

// 
typedef struct command {
  char *argv[MAX_COMMAND_ARGS];
  command_handler *command;
  int argc;
} command_object;
void print_help();
// used to check if the provided string is a flag
bool is_flag_argument(char *arg);
// returns a prepared command_object to execute user input
command_object *new_command_object();
// a command to generate a new zlog configuration
command_handler *new_zlog_config_command(command_object *self);
void new_logger_config_wrapper();


// NOTE: you will want to add additional statements for any other commands that are being included
void print_help() {
  printf("CLI HELP MENU\n-------------\n\n");
  printf("new-zlog-config <path-to-config> (generate a new zlog config file)\n");
}

void new_logger_config_wrapper(command_object *self) {
  char *config_path = self->command->argv[0];
  if (config_path == NULL) {
    config_path = "zlog.conf";
  }
  printf("saving zlog config to %s\n", config_path);
  int response = new_logger_config(config_path);
  if (response != 0) {
    printf("failed call new_logger_config");
  }
  // TODO(bonedaddy): use dyanmic argument
  printf("saved zlog config to %s\n", config_path);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
command_handler *new_zlog_config_command(command_object *self) {
  // allocate size of command_handler and the new-log-config `char *`
  command_handler *handler = malloc(sizeof(command_handler) + sizeof("new-zlog-config"));
  handler->callback = new_logger_config_wrapper;
  handler->name = "new-zlog-config";
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
  // parse cli arguments provided by user 
  for (int i = 0; i < argc; i++) {
    // we only accept whole commands or flag with `--argName`
    /* if (strlen(argv[i]) >= 2) {
    // check to see if arg is a command line flag
     /if (is_flag_argument(argv[i])) {
        // TODO(bonedaddy): handle
        printf("found flag: %s\n", argv[i]);
      }
    }
    */
    pcobj->argv[i] = argv[i];
    pcobj->argc = argc;
  }
  // iterate over arguments to match against a command
  for (int i = 0; i < argc; i++) {
    // handle new-zlog-config
    // next argument should be the path to the config file
    if (strcmp(argv[i], "new-zlog-config") == 0) {
      int idx = i + 1;
      if (argc < (idx+1)) {
        printf("invalid number of arguments provided. got %i want %i\n", argc, idx+1);
        return NULL;
      }
      char *config_path = argv[idx];
      pcobj->argv[0] = config_path;
      printf("config path %s\n", pcobj->argv[0]);
      command_handler *zlg = new_zlog_config_command(pcobj);
      command_handler *zlog_command = malloc(sizeof(zlg));
      zlog_command = zlg;
      pcobj->command = zlog_command;
    }
/*    ADD YOUR NEW COMMAND CHECKS HERE
*/
  }
  return pcobj;
}

int main(int argc, char *argv[]) {
  command_object *pcmd = new_command_object(argc, argv);
  if (pcmd == NULL) {
    printf("failed to get command_object");
    return - 1;
  }
  pcmd->command->callback(pcmd);
}