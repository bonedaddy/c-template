#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <assert.h>
#include "command_line.c"

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
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

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_is_flag_argument),
        cmocka_unit_test(test_new_command_object),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}