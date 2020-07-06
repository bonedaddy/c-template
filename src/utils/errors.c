#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdatomic.h>
#include "../../include/utils/errors.h"

cg_error *new_cg_error(char *message) {
    cg_error *err = calloc(sizeof(cg_error), sizeof(cg_error) + strlen(message) + 1);
    if (err == NULL) {
        printf("failed to calloc cg_error\n");
        return NULL;
    }
    err->message = cg_error_string(message);
    return err;
}

char *cg_error_string(char *message) {
    char *msg = calloc(sizeof(char), strlen(message) + 1);
    if (msg == NULL) {
        printf("failed to calloc msg\n");
        return NULL;
    }
    // TODO(bonedaddy): should we use strcpy??
    memmove(msg, message, strlen(message) + 1);
    return msg;
}

int wrap_cg_error(cg_error *error, char *message) {
    // 4 for delimiter of ` | `
    error->message = realloc(error->message, strlen(error->message) + strlen(message) + 4);
    if (error->message == NULL) {
        return -1;
    }
    strcat(error->message, " | ");
    strcat(error->message, message);
    return 0;
}

void free_cg_error(cg_error *error) {
    free(error->message);
    free(error);
}