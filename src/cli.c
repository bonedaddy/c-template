#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <argtable3.h>
#include "../include/utils/colors.h"
#include "../include/utils/command_line.h"
#include "../include/utils/safe_mem.h"
#include "../include/utils/logger.h"


#ifndef COMMAND_VERSION_STRING
#define COMMAND_VERSION_STRING "0.0.1"
#endif

// a command to generate a new zlog configuration
command_handler *new_zlog_config_command(command_object *self);
// displays the help command
command_handler *new_help_command(command_object *self);
// wrapper function to use as the calback
void new_zlog_config_callback(int argc, char *argv[]);

#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
// ignore the uninitialized warning about config_path
void new_zlog_config_callback(int argc, char *argv[]) {
  char *config_path;
  if (argc == 1) {
    config_path = argv[0];
  } else {
    print_colored(COLORS_YELLOW, "[warn] no config path given defaulting to zlog.conf\n");
    config_path = "zlog.conf";
  }
  int response = new_logger_config(config_path);
  if (response != 0) {
    print_colored(COLORS_RED, "[error] failed to call new_logger_config");
  } else {
    printf("%s[info] zlog config saved to %s\n", ANSI_COLOR_GREEN, config_path);
  }
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
  setup_args(COMMAND_VERSION_STRING);
  void *argtable[] = {help, version, file, output, command_to_run, end};
  int response = parse_args(argc, argv, argtable);
  switch (response) {
    case 0:
      break;
    case -1:
      printf("parse_args failed\n");
      return response;
    case -2: // this means --help was invoked
      return 0;
  }
  // handle help if no other cli arguments were given (aka binary invoked with ./some-binary)
  if (argc == 1) {
    print_help(argv[0], argtable);
    return 0;
  }
  // construct the command object
  command_object *pcmd = new_command_object(argc, argv);
  if (pcmd == NULL) {
    printf("failed to get command_object");
    return -1;
  }
  // load command handler
  int resp = load_command(pcmd, new_zlog_config_command(pcmd));
  if (resp != 0) {
    printf("failed to load help command");
    return -1;
  }
  // get the command we are running
  char *run_command = get_run_command();
  // START COMMAND INPUT PREPARATION 
  // - here we parse the command being ran, and prepare inputs for passing into the callback function
  if (strcmp(*command_to_run->sval, "new-zlog-config") == 0) {
    // if no output file was given, set argc to 0 which will trigger default filename of zlog.confm
    if (strlen((char *)*output->filename) == 0) {
      pcmd->argc = 0;
    } else {
      pcmd->argc = 1;
      pcmd->argv[0] = realloc(pcmd->argv[0], sizeof(pcmd->argv[0]) + sizeof(*command_to_run->sval));
      // typecast as filename is  pointer to a constant char
      pcmd->argv[0] = (char *)*output->filename;
    }
  }
  // END COMMAND INPUT PREPARATION
  resp = execute(pcmd, run_command);
  if (resp != 0) {
    // TODO(bonedaddy): figure out if we should log this
    // printf("command run failed\n");
  }
  free_command_object(pcmd);
  arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
  return resp;
}