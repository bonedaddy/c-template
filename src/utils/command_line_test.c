#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <assert.h>
#include "command_line.c"

void print_hello_world();
void print_hello_world() { 
    printf("hello world"); 
}
command_handler *new_test_command();
#pragma GCC diagnostic ignored "-Wanalyzer-malloc-leak"
command_handler *new_test_command() {
    command_handler *handler = malloc(sizeof(command_handler));
    if (handler == NULL) {
        printf("failed to malloc comand_handler\n");
        return NULL;
    }
    handler->callback = print_hello_world;
    handler->name = "hello-world";
    return handler;
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wanalyzer-malloc-leak"
void test_new_command_object(void **state) {
    int argc = MAX_COMMAND_ARGS;
    // increase argc beyond the maximum limit
    argc++;
    char *argv[1] = {"test_arg"};
    command_object *cmdobj = new_command_object(argc, argv);
    assert(cmdobj == NULL);
    argc = 1;
    cmdobj = new_command_object(argc, argv);
    assert(cmdobj != NULL);
    assert(cmdobj->argc == 1);
    assert(strcmp(cmdobj->argv[0], "test_arg") == 0);
    free_command_object(cmdobj);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
// for some reason gcc complains response isn't used
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#pragma GCC diagnostic ignored "-Wanalyzer-malloc-leak"
void test_load_command(void **state) {
    int argc = 1;
    char *argv[MAX_COMMAND_ARGS] = {"hello"};
    command_object *cmdobj = new_command_object(argc, argv);
    assert(cmdobj != NULL);
    command_handler *cmd = new_test_command();
    int response = load_command(cmdobj, cmd);
    assert(response == 0);
    assert(cmdobj->command_count == 1);
    int old_count = cmdobj->command_count;
    cmdobj->command_count = 33;
    response = load_command(cmdobj, new_test_command());
    assert(response == -1);
    free_command_object(cmdobj);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
// for some reason gcc complains response isn't used
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#pragma GCC diagnostic ignored "-Wanalyzer-malloc-leak"
void test_execute(void **state) {
    int argc = 1;
    char *argv[MAX_COMMAND_ARGS] = {"hello"};
    command_object *cmdobj = new_command_object(argc, argv);
    assert(cmdobj != NULL);
    command_handler *cmd = new_test_command();
    int response = load_command(cmdobj, cmd);
    assert(response == 0);
    response = execute(cmdobj, "hello-world");
    assert(response == 0);
    response = execute(cmdobj, "not-a-command");
    assert(response == -1);
    free_command_object(cmdobj);
}

#pragma GCC diagnostic ignored "-Wanalyzer-malloc-leak"
int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_new_command_object),
        cmocka_unit_test(test_load_command),
        cmocka_unit_test(test_execute),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}