#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <assert.h>
#include "command_line.c"

void print_hello_world();
void print_hello_world() { printf("hello world"); }
command_handler *new_test_command();
command_handler *new_test_command() {
    command_handler *handler = malloc(sizeof(command_handler) + sizeof("hello-world"));
    handler->callback = print_hello_world;
    handler->name = "hello-world";
    return handler;
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
// for some reason the compiler things is_flag is used
void test_is_flag_argument(void **state) {
    bool is_flag = is_flag_argument("--config");
    assert(is_flag == true);
    is_flag = is_flag_argument("l");
    assert(is_flag == false);
    is_flag = is_flag_argument("ll");
    assert(is_flag == false);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
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
    free(cmdobj);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void test_load_command(void **state) {
    int argc = 1;
    char *argv[MAX_COMMAND_ARGS] = {"hello"};
    command_object *cmdobj = new_command_object(argc, argv);
    assert(cmdobj != NULL);
    int response = load_command(cmdobj, *new_test_command());
    assert(response == 0);
    int old_count = cmdobj->command_count;
    cmdobj->command_count = 33;
    response = load_command(cmdobj, *new_test_command());
    assert(response == -1);
  // this causes an error
  //  free_command_object(cmdobj);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_is_flag_argument),
        cmocka_unit_test(test_new_command_object),
        cmocka_unit_test(test_load_command),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}