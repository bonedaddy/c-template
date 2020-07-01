#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <argtable3.h>
#include <string.h>
#include "../include/cli.h"
#include "../include/utils/array_len.h"
#include "../include/utils/logger.h"
#include "../include/utils/safe_mem.h"

void print_help() {
  printf("CLI HELP MENU\n-------------\n\n");
  printf("new-zlog-config <path-to-config> (generate a new zlog config file - default to zlog.conf)\n");
  printf("help\t\t\t\t (print command help)\n");
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
// print_help callback function
void print_help_callback(int argc, char *argv[]) {
  print_help();
}


// wraps new_logger_config for zlo
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
  // parse cli arguments provided by user  and assign to pcobj
  for (int i = 0; i < argc; i++) {
    pcobj->argv[i] = argv[i];
    pcobj->argc = argc;
  }
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
      command_handler *zlg = new_zlog_config_command(pcobj);
      command_handler *zlog_command = malloc(sizeof(zlg));
      zlog_command = zlg;
      pcobj->command = zlog_command;
    }
    // handle help command (triggered via `help` or no arguments)
    if (strcmp(argv[i], "help") == 0 || argc == 1) {
      command_handler *hlp = new_help_command(pcobj);
      command_handler *help_command = malloc(sizeof(hlp));
      help_command = hlp;
      pcobj->command = help_command;
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