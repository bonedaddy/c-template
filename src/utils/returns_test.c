#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <assert.h>
#include "../../include/utils/errors.h"
#include "../../include/utils/returns.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
void test_cg_return_no_error(void **state) {
    // ensures that no error passed in results in a NULL err
    int a = 10;
    cg_return *ret_val = new_cg_return(&a, NULL);
    assert(ret_val != NULL);
    assert(ret_val->value != NULL);
    assert(ret_val->err == NULL);
    int *aa = (int *)ret_val->value;
    assert(*aa == a);
    free_cg_return(ret_val);
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"
void test_cg_return_error(void **state) {
    char *msg = "thisisanerrormessage";
    // ensures that the correct error is passed through
    cg_error *err = new_cg_error(msg);
    assert(err != NULL);
    cg_return *ret_val = new_cg_return(NULL, err);
    assert(ret_val != NULL);
    assert(
        strcmp(msg, ret_val->err->message) == 0
    );
    free_cg_return(ret_val);
}


int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_cg_return_no_error),
        cmocka_unit_test(test_cg_return_error),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
