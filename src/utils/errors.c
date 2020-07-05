#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>
#include "../../include/utils/errors.h"

cg_error *new_cg_error(char *message) {
    cg_error *err = calloc(sizeof(cg_error), sizeof(cg_error) + strlen(message));
    err->message = cg_error_string(message);
    return err;
}

char *cg_error_string(char *message) {
    char *msg = calloc(sizeof(char), strlen(message) + 1);
    // TODO(bonedaddy): should we use strcpy??
    memmove(msg, message, strlen(message) + 1);
    return msg;
}

int wrap_cg_error(cg_error *error, char *message) {
    error->message = realloc(error->message, strlen(error->message) + strlen(message) + 1);
    if (error->message == NULL) {
        return -1;
    }
    strcat(error->message, message);
    return 0;
}

void free_cg_error(cg_error *error) {
    free(error->message);
    free(error);
}

int main(void) {
    cg_error *err = new_cg_error("hello");
    printf("%s\n", err->message);
    int response = wrap_cg_error(err, " world");
    if (response == -1) {
        printf("failed to wrap error");
        return response;
    }
    printf("%s\n", err->message);
    response = wrap_cg_error(err, " world");
    if (response == -1) {
        printf("failed to wrap error");
        return response;
    }
    printf("%s\n", err->message);
    response = wrap_cg_error(err, " world");
    if (response == -1) {
        printf("failed to wrap error");
        return response;
    }
    printf("%s\n", err->message);
    free_cg_error(err);
}