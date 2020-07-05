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
    // thl->lock = fn_mutex_lock;
    thl->lock = pthread_mutex_lock;
    thl->unlock = pthread_mutex_unlock;
    thl->log = log_func;
    thl->debug = with_debug;
    pthread_mutex_init(&thl->mutex, NULL);
    return thl;
}

file_logger *new_file_logger(char *output_file, bool with_debug) {
    thread_logger *thl = new_thread_logger(with_debug);
    file_logger *fhl = malloc(sizeof(file_logger));
    // append to file, create if not exist, sync write files
    // TODO(bonedaddy): try to use O_DSYNC for data integrity sync
    int file_descriptor = open(output_file, O_WRONLY | O_CREAT | O_SYNC | O_APPEND);
    if (file_descriptor <= 0) {
        thl->log(thl, 0, "failed to run posix open function", LOG_LEVELS_ERROR);
        return NULL;
    }
    fhl->file_descriptor = file_descriptor;
    fhl->thl = thl;
    return fhl;
}

int write_file_log(int file_descriptor, char *message) {
    // 2 for \n
    char *msg = calloc(sizeof(char), strlen(message) + 2);
    strcat(msg, message);
    strcat(msg, "\n");
    //strcat(msg, message);
    // strcat(msg, "\n");
    int response = write(file_descriptor, msg, strlen(msg));
    if (response == -1) {
        printf("failed to write file log message");
        return response;
    }
    free(msg);
    return 0;
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
    msg[0] = '[';
    msg[1] = 'i';
    msg[2] = 'n';
    msg[3] = 'f';
    msg[4] = 'o';
    msg[5] = ']';
    msg[6] = ' ';
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
    msg[0] = '[';
    msg[1] = 'w';
    msg[2] = 'a';
    msg[3] = 'r';
    msg[4] = 'n';
    msg[5] = ']';
    msg[6] = ' ';
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
    msg[0] = '[';
    msg[1] = 'e';
    msg[2] = 'r';
    msg[3] = 'r';
    msg[4] = 'o';
    msg[5] = 'r';
    msg[6] = ']';
    msg[7] = ' ';
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
    msg[0] = '[';
    msg[1] = 'd';
    msg[2] = 'e';
    msg[3] = 'b';
    msg[4] = 'u';
    msg[5] = 'g';
    msg[6] = ']';
    msg[7] = ' ';
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