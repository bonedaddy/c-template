#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>
#include <pthread.h>
#include <unistd.h>
#include <assert.h>
#include "../../include/sync/wait_group.h"

wait_group_t *wait_group_new() {
    wait_group_t *wg = malloc(sizeof(wait_group_t));
    if (wg == NULL) {
        return NULL;
    }
    wg->active_processes = 0;
    pthread_mutex_init(&wg->mutex, NULL);
    return wg;
}

void wait_group_wait(wait_group_t *wg) { 
    for (;;) {
        // sleep for 750 milisecond
        sleep(0.75);
        pthread_mutex_lock(&wg->mutex);
        int count = wg->active_processes;
        pthread_mutex_unlock(&wg->mutex);
        if (count == 0) {
            break;
        }
    }
    pthread_mutex_destroy(&wg->mutex);
    free(wg);
}

void wait_group_add(wait_group_t *wg, int count) {
    pthread_mutex_lock(&wg->mutex);
    wg->active_processes += count;
    pthread_mutex_unlock(&wg->mutex);
}

int wait_group_done(wait_group_t *wg) {
    // we will set to -1 if caller is trying to decrease count below 0
    int rc = 0;
    pthread_mutex_lock(&wg->mutex);
    // should we guard this with an assert and use compile time NODEBUG ?
    if (wg->active_processes <= 0) {
        printf("[warn] invalid call to wait_group_done with 0 active processes\n");
        rc = -1;
    } else {
        // decrease count by 1
        wg->active_processes--;
    }
    pthread_mutex_unlock(&wg->mutex);
    return rc;
}
