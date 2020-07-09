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
    pthread_mutex_init(&wg->cond_mutex, NULL);
    pthread_cond_init(&wg->cond_var, NULL);
    return wg;
}

void wait_group_recv_signal(wait_group_t *wg) {
    pthread_mutex_lock(&wg->cond_mutex);
    pthread_cond_wait(&wg->cond_var, &wg->cond_mutex);
    pthread_mutex_lock(&wg->cond_mutex);
}

void wait_group_send_signal(wait_group_t *wg) {
    pthread_mutex_lock(&wg->cond_mutex);
    pthread_cond_signal(&wg->cond_var);
    pthread_mutex_unlock(&wg->cond_mutex);
}

void wait_group_reset_signal(wait_group_t *wg) {
    // signal anyone waiting on the current cond_var
    // we do this before replacing it
    pthread_mutex_lock(&wg->cond_mutex);
    pthread_cond_signal(&wg->cond_var);
    pthread_mutex_unlock(&wg->cond_mutex);
    // re init
    pthread_mutex_lock(&wg->cond_mutex);
    pthread_cond_init(&wg->cond_var, NULL);
    pthread_mutex_unlock(&wg->cond_mutex);
}

void wait_group_wait(wait_group_t *wg) { 
    for (;;) {
        // sleep for 750 milisecond
        sleep(0.75);
        // NOTE(bonedaddy): iirc we dont need this
        // pthread_mutex_lock(&wg->mutex);
        int count = wg->active_processes;
        // NOTE(bonedaddy): iirc we dont need this
        // pthread_mutex_unlock(&wg->mutex);
        if (count == 0) {
            break;
        }
    }
    // lock before destroy
    pthread_mutex_lock(&wg->mutex);
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
    // TODO(bonedaddy): decide if we should exit here, as this logic would only trigger
    // if there was an error in program design
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
