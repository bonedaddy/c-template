#include <stdlib.h>
#include "../../include/utils/errors.h"
#include "../../include/utils/returns.h"

cg_return *new_cg_return(void *value, cg_error *err) {
    cg_return *ret = calloc(sizeof(cg_return), sizeof(value) + sizeof(err));
    if (err != NULL) {
        ret->err = err;
    } else {
        ret->err = NULL;
    }
    if (value != NULL) {
        ret->value = value;
    } else {
        ret->value = NULL;
    }
    return ret;
}

void free_cg_return(cg_return *ret) {
    // TODO(bonedaddy): determine how we can determine if ret->value needs to be freed
    if (ret->err != NULL) {
        free(ret->err);
    }
    free(ret);
}

int main(void) {
    int a = 1;
    cg_error *err = new_cg_error("hello world");
    cg_return *ret_val = new_cg_return(&a, err);
    printf("%i\n", *(int *)ret_val->value);
    printf("%s\n", ret_val->err->message);
    free_cg_return(ret_val);
}