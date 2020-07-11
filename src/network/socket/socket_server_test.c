#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
// sys/time.h is needed for the timeval
//  #include <time.h>
#include <stdbool.h>
#include <pthread.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/time.h>
#include "../../../include/utils/logger.h"
#include "../../../include/sync/wait_group.h"
#include "../../../include/network/socket/socket.h"
#include "../../../include/network/socket/socket_server.h"

int main(int argc, char **argv) {
    setup_signal_handling();

    thread_logger *thl = new_thread_logger(false);
    if (thl == NULL) {
        printf("new_thread_logger failed\n");
        return -1;
    }
    if (argc >= 2) {
        if (strcmp(argv[1], "client") == 0) {
            addr_info client_hints = default_hints();
            socket_client *sock_client = new_socket_client(thl, client_hints, "localhost", "8081");
            int rc = socket_send(
                thl, 
                sock_client->socket_number, 
                "hello world"
            );
            if (rc == -1) {
                thl->log(thl, 0, "failed to send message", LOG_LEVELS_ERROR);
                close(sock_client->socket_number);
                return -1;
            }
            char *msg = socket_recv(thl, sock_client->socket_number);
            thl->logf(thl, 0, LOG_LEVELS_INFO, "received message: %s\n", msg);
            close(sock_client->socket_number);
            return 0;
        }
    }

    addr_info hints = default_hints();
    char *port;
    switch (argc) {
        case 1:
            port = "8081";
            break;
        case 2:
            port = argv[1]; // 2nd element
            break;
        default:
            thl->log(thl, 0, "improper command invocation\nusage: ./socketserver <port-num>\tport-num: default 8081", LOG_LEVELS_WARN);
            clear_thread_logger(thl);
            return -1;
    }
    socket_server *srv = new_socket_server(hints, thl, 100, port);
    if (srv == NULL) {
        srv->log(thl, 0, "socket server creation failed", LOG_LEVELS_ERROR);
        clear_thread_logger(thl);
        return -1;
    }
    wait_group_add(srv->wg, 1);
    pthread_create(
        &srv->thread,
        // &srv->taddr,
        NULL,
        async_listen_func,
        srv
    );
    wait_group_wait(srv->wg);
    srv->log(srv->thl, 0, "caught exit signal, exiting", LOG_LEVELS_INFO);
    close(srv->socket_number);
    // wait for main async listen func thread to finish
    pthread_join(srv->thread, NULL);
    srv->log(srv->thl, 0, "goodbye", LOG_LEVELS_INFO);
    clear_thread_logger(srv->thl);
    free(srv);
}