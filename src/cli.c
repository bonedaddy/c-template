#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "../include/utils/command_line.h"
#include "../include/utils/safe_mem.h"
#include "../include/utils/logger.h"

// prints command help
void print_help();
void match_command(command_object *pcobj, int argc, char *argv[]);
// a command to generate a new zlog configuration
command_handler *new_zlog_config_command(command_object *self);
// displays the help command
command_handler *new_help_command(command_object *self);
// wrapper function to use as the calback
void new_zlog_config_callback(int argc, char *argv[]);
// wrapper function to use as the callbacK
void print_help_callback(int argc, char *argv[]);


void print_help() {
  printf("CLI HELP MENU\n-------------\n\n");
  printf("new-zlog-config <path-to-config> (generate a new zlog config file - default to zlog.conf)\n");
  printf("help\t\t\t\t (print command help)\n");
}

void match_command(command_object *pcobj, int argc, char *argv[]) {
  // iterate over arguments to match against a command
  for (int i = 0; i < argc; i++) {
    // handle new-zlog-config
    // next argument should be the path to the config file
    // if there is not provided argument we default to zlog.conf
    if (strcmp(argv[i], "new-zlog-config") == 0) {
      int idx = i + 1;
      char *config_path;
      if (argc < (idx+1)) {
        config_path = "zlog.conf";
      } else {
        config_path = argv[idx];
      }
      // NOTE(bonedaddy): looks like this isn't being properly set and passed through to callback
      pcobj->argv[0] = config_path;
      pcobj->argc = 1;
      printf("config path %s\n", pcobj->argv[0]);
      // temporary variable to hold command handler
      command_handler *zlg = new_zlog_config_command(pcobj);
      // allocate a chunk of memory to use as a permanent variable for command handler
      command_handler *zlog_command = malloc(sizeof(zlg));
      // move from temp variable to permanent variable
      zlog_command = zlg;
      pcobj->command = zlog_command;
    }
    // handle help command (triggered via `help` or no arguments)
    if (strcmp(argv[i], "help") == 0 || argc == 1) {
      // temporary variable to hold command handler
      command_handler *hlp = new_help_command(pcobj);
      // allocate a chunk of memory to use as a permanent variable for command handler
      command_handler *help_command = malloc(sizeof(hlp));
      // move from temp variable to permanent variable
      help_command = hlp;
      pcobj->command = help_command;
    }
    /*    ADD YOUR NEW COMMAND CHECKS HERE
    */
  }
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void print_help_callback(int argc, char *argv[]) {
  print_help();
}

void new_zlog_config_callback(int argc, char *argv[]) {
  char *config_path;
  if (argc == 1) {
    config_path = malloc(sizeof(argv[0]));
    config_path = argv[0];
  }
  if (config_path == NULL) {
    printf("no config path given defaulting to zlog.conf\n");
    config_path = "zlog.conf";
  }
  printf("zlog config saving to %s\n", config_path);
  int response = new_logger_config(config_path);
  if (response != 0) {
    printf("failed call new_logger_config");
  }
  printf("zlog config saved to %s\n", config_path);
}


#pragma GCC diagnostic ignored "-Wunused-parameter"
command_handler *new_help_command(command_object *self) {
    // allocate size of command_handler and the new-log-config `char *`
  command_handler *handler = malloc(sizeof(command_handler) + sizeof("help"));
  handler->callback = print_help_callback;
  handler->name = "help";
  return handler;
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
command_handler *new_zlog_config_command(command_object *self) {
  // allocate size of command_handler and the new-log-config `char *`
  command_handler *handler = malloc(sizeof(command_handler) + sizeof("new-zlog-config"));
  handler->callback = new_zlog_config_callback;
  handler->name = "new-zlog-config";
  return handler;
}


int main(int argc, char *argv[]) {
  command_object *pcmd = new_command_object(argc, argv);
  if (pcmd == NULL) {
    printf("failed to get command_object");
    return - 1;
  }
  // match the command object
  match_command(pcmd, argc, argv);
  // create a safe memory wrapper around the command object
  memory_object memobj = new_memory_object(pcmd);
  pcmd->command->callback(pcmd->argc, pcmd->argv);
  int response = free_memory_object(&memobj);
  if (response != 0) {
    printf("failed to free memory object\n");
    return response;
  }
  return 0;
}