#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include "../../include/utils/logger.h"

thread_logger *new_thread_logger(bool with_debug) {
    thread_logger *thl = malloc(sizeof(thread_logger));
    if (thl == NULL) {
        printf("failed to malloc thread_logger\n");
        return NULL;
    }
    thl->lock = pthread_mutex_lock;
    thl->unlock = pthread_mutex_unlock;
    thl->log = log_func;
    thl->debug = with_debug;
    pthread_mutex_init(&thl->mutex, NULL);
    return thl;
}

file_logger *new_file_logger(char *output_file, bool with_debug) {
    /*
        TODO(bonedaddy): im not sure how I like the goto and label for error cleaning up
        it seems to make the code a bit harder to understand
    */
    thread_logger *thl = new_thread_logger(with_debug);
    if (thl == NULL) {
        // dont printf log here since new_thread_logger handles that
        goto HANDLE_ERROR;
    }
    file_logger *fhl = malloc(sizeof(file_logger));
    if (fhl == NULL) {
        printf("failed to malloc file_logger\n");
        goto HANDLE_ERROR;
    }
    // append to file, create if not exist, sync write files
    // TODO(bonedaddy): try to use O_DSYNC for data integrity sync
    int file_descriptor = open(output_file, O_WRONLY | O_CREAT | O_SYNC | O_APPEND, 0640);
    // TODO(bonedaddy): should this just be `< 0` ? `open` shouldn't return 0 but im unsure about removing the check for it
    if (file_descriptor <= 0) {
        printf("failed to run posix open function\n");
        goto HANDLE_ERROR;
    }
    fhl->file_descriptor = file_descriptor;
    fhl->thl = thl;
    return fhl;
    HANDLE_ERROR:
        if (thl != NULL) {
            free(thl);
        }
        if (fhl != NULL) {
            free(fhl);
        }
        return NULL;
}

int write_file_log(int file_descriptor, char *message) {
    // 2 for \n
    char *msg = calloc(sizeof(char), strlen(message) + 2);
    strcat(msg, message);
    strcat(msg, "\n");
    int response = write(file_descriptor, msg, strlen(msg));
    if (response == -1) {
        printf("failed to write file log message");
    } else {
        // this branch will be triggered if write doesnt fail
        // so overwrite the response to 0 as we want to return 0 to indicate
        // no error was received, and returning response directly would return the number of bytes written
        response = 0;
    }
    free(msg);
    return response;
}

void log_func(thread_logger *thl, int file_descriptor, char *message, LOG_LEVELS level) {
    switch (level) {
        case LOG_LEVELS_INFO:
            info_log(thl, file_descriptor, message);
            break;
        case LOG_LEVELS_WARN:
            warn_log(thl, file_descriptor, message);
            break;
        case LOG_LEVELS_ERROR:
            error_log(thl, file_descriptor, message);
            break;
        case LOG_LEVELS_DEBUG:
            debug_log(thl, file_descriptor, message);
            break;
    }
}

void info_log(thread_logger *thl,  int file_descriptor, char *message) {
    thl->lock(&thl->mutex);
    // 2 = 1 for null terminator, 1 for space after ]
    char *msg = calloc(sizeof(char), strlen(message) + strlen("[info]") + (size_t)2);
    strcat(msg, "[info] ");
    strcat(msg, message);
    if (file_descriptor != 0) {
        write_file_log(file_descriptor, msg);
    }
    print_colored(COLORS_GREEN, msg);
    thl->unlock(&thl->mutex);
    free(msg);
}

void warn_log(thread_logger *thl, int file_descriptor, char *message) {
    thl->lock(&thl->mutex);
    // 2 = 1 for null terminator, 1 for space after ]
    char *msg = calloc(sizeof(char), strlen(message) + strlen("[warn]") + (size_t) 2);
    strcat(msg, "[warn] ");
    strcat(msg, message);
    if (file_descriptor != 0) {
        // TODO(bonedaddy): decide if we want to copy
        // char *cpy = malloc(strlen(msg)+1);
        // strcpy(cpy, msg);
        write_file_log(file_descriptor, msg);
        // free(cpy);
    }
    print_colored(COLORS_YELLOW, msg);
    thl->unlock(&thl->mutex);
    free(msg);
}

void error_log(thread_logger *thl, int file_descriptor, char *message) {
    thl->lock(&thl->mutex);
    // 2 = 1 for null terminator, 1 for space after ]
    char *msg = calloc(sizeof(char), strlen(message) + strlen("[error]") + (size_t)2);
    strcat(msg, "[error] ");
    strcat(msg, message);
    if (file_descriptor != 0) {
        write_file_log(file_descriptor, msg);
    }
    print_colored(COLORS_RED, msg);
    thl->unlock(&thl->mutex);
    free(msg);
}

void debug_log(thread_logger *thl, int file_descriptor, char *message) {
    // unless debug enabled dont show
    if (thl->debug == false) {
        return;
    }
    thl->lock(&thl->mutex);
    // 2 = 1 for null terminator, 1 for space after ]
    char *msg = calloc(sizeof(char), strlen(message) + strlen("[debug]") + (size_t) 2);
    strcat(msg, "[debug] ");
    strcat(msg, message);
    if (file_descriptor != 0) {
        //char *cpy = malloc(strlen(msg));
        //strcpy(cpy, msg);
        write_file_log(file_descriptor, /*cpy*/ msg);
    }
    print_colored(COLORS_SOFT_RED, msg);
    thl->unlock(&thl->mutex);
    free(msg);
}

void clear_thread_logger(thread_logger *thl) {
    free(thl);
}

void clear_file_logger(file_logger *fhl) {
    close(fhl->file_descriptor);
    clear_thread_logger(fhl->thl);
    free(fhl);
}