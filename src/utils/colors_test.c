#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <assert.h>
#include <string.h>
#include "../../include/utils/colors.h"

typedef struct args {
    COLORS test_color;
    char *want_ansi;
} args;
typedef struct test {
    args args;
    char *name;
} test;

void test_print_color(void **state);
void test_get_ansi_color_scheme(void **state);
void validate_test_args(test testdata);
// TODO(bonedaddy): test format_colored

void validate_test_args(test testdata) {
    //   printf("%s\n", format_colored(testdata.args.test_color, testdata.name));
    char *scheme = get_ansi_color_scheme(testdata.args.test_color);
    if (strcmp(scheme, testdata.args.want_ansi) == 0) {

        printf("%stest %s passed\n", scheme, testdata.name);
        return;
    }
    printf("test %s failed\n", testdata.name);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void test_get_ansi_color_scheme(void **state) {

    test tests[7] = {
        {
            .name = "red", 
            .args = {
                .test_color = COLORS_RED,
                .want_ansi = ANSI_COLOR_RED,
            },
        },
        {
            .name = "green", 
            .args = {
                .test_color = COLORS_GREEN,
                .want_ansi = ANSI_COLOR_GREEN,
            },
        },
        {
            .name = "yellow", 
            .args = {
                .test_color = COLORS_YELLOW,
                .want_ansi = ANSI_COLOR_YELLOW,
            },
        },
        {
            .name = "blue", 
            .args = {
                .test_color = COLORS_BLUE,
                .want_ansi = ANSI_COLOR_BLUE,
            },
        },
        {
            .name = "magenta", 
            .args = {
                .test_color = COLORS_MAGENTA,
                .want_ansi = ANSI_COLOR_MAGENTA,
            },
        },
        {
            .name = "cyan", 
            .args = {
                .test_color = COLORS_CYAN,
                .want_ansi = ANSI_COLOR_CYAN,
            },
        },
        {
            .name = "reset", 
            .args = {
                .test_color = COLORS_RESET,
                .want_ansi = ANSI_COLOR_RESET,
            },
        },
    };
    for (int i = 0; i < 7; i++) {
        validate_test_args(tests[i]);
    }
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_get_ansi_color_scheme),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}