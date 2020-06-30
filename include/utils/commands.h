#include <stdio.h>
#include "commander.h"
#include "array_len.h"

static void new_config_command(command_t *self) {
    printf("new_config_command called\n");
    printf("argc: %i\n", self->argc);
    printf("START argv\n");
    for (int i = 0; i < array_len(self->argv); i++) {
        printf("%s\n", self->argv[i]);
    }
    printf("END argv\n");

}

// should save returned variable in command_object
command_t  *setup_command_object(char *name, char *version) {
    command_t cmd;
    command_init(&cmd, name, version);
    command_option(&cmd, "-ncfg", "--new-config", "create config file", new_config_command);
    return &cmd;
}


int main(int argc, char **argv) {
    command_t *command_object = setup_command_object("test-command", "0.0.1");
    command_parse(command_object, argc, argv);
    return 0;
}