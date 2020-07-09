// this is needed otherwise compilation errors occur
// https://stackoverflow.com/questions/39409846/why-does-gcc-not-complain-about-htons-but-complains-about-getaddrinfo-when-c/39410095#39410095
// https://man7.org/linux/man-pages/man3/getaddrinfo.3.html
#define _POSIX_C_SOURCE 201112L
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
#include "../../include/utils/logger.h"
#include "../../include/sync/wait_group.h"
#include "../../include/network/socket.h"

/*
    * the following two variables are not set
    * in the header file to prevent accidental use
    * while accessible from within the source file
    * if you are just importing the header, they will not be available
*/
pthread_mutex_t _signal_mutex;
// indicates we should begin cleanup processes
bool _do_exit;


/** Returns true on success, or false if there was an error */
/* https://stackoverflow.com/questions/1543466/how-do-i-change-a-tcp-socket-to-be-non-blocking/1549344#1549344 */
bool set_socket_blocking_status(int fd, bool blocking) {
    if (fd < 0) {
        return false;
    } else {
        int flags = fcntl(fd, F_GETFL, 0);
        if (flags == -1) {
            return false;
        }
        flags = blocking ? (flags & ~O_NONBLOCK) : (flags | O_NONBLOCK);
        return (fcntl(fd, F_SETFL, flags) == 0) ? true : false;
    }
}

#pragma GCC diagnostic ignored "-Wunused-parameter"
void signal_handler_fn(int signal_number) {
    printf("\n"); // print new line so terminal doesn't display ^c on a line with a log message
    pthread_mutex_lock(&_signal_mutex);
    _do_exit = true;
    pthread_mutex_unlock(&_signal_mutex);
}

void print_and_exit(int error_number) {
    printf("failure detected: %s\n", strerror(error_number));
    exit(-1);
}


void *async_handle_conn_func(void *data) {
    conn_handle_data *chdata = (conn_handle_data *)data;
    fd_set socket_set;
    FD_ZERO(&socket_set); // always zero out before use
    FD_SET(chdata->conn->socket_number, &socket_set);
    // FD_CLR() // used to remove from set
    // must provide a number larger than last socket descriptor
    int max_socket = chdata->conn->socket_number + 1;
    // copy since select() modifies stuff
    fd_set socket_set_copy;
    socket_set_copy = socket_set;
    // set a timeout of 1 second
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    int rc = select(
        max_socket, 
        &socket_set_copy, 
        0, 
        0, 
        &timeout
    );
    if (rc == 0 || rc == -1) {
        chdata->srv->log(chdata->srv->thl, 0, "connection not ready before timeout", LOG_LEVELS_WARN);
    } else {
        for (;;) {
            if (_do_exit == true) {
                // break out of loop trigger closure
                break;
            }
            // once select returns the copy contains the sockets which are ready to be read from
            if (FD_ISSET(chdata->conn->socket_number, &socket_set_copy)) {
                char request[1024];
                rc = recv(chdata->conn->socket_number, request, sizeof(request), 0);
                if (rc == -1 || rc == 0) {
                    break;
                }
                rc = send(
                    chdata->conn->socket_number,
                    request,
                    strlen(request),
                    0
                );
                if ((size_t)rc < strlen(request)) {
                    chdata->srv->log(chdata->srv->thl, 0, "failed to send enough bytes", LOG_LEVELS_WARN);
                }
            }
        }
    }
    close(chdata->conn->socket_number);
    chdata->srv->log(chdata->srv->thl, 0, "client disconnected", LOG_LEVELS_INFO);
   // decrease the wait group counter as this thread is no longer active
    wait_group_done(chdata->srv->wg);
    // free up resources associated with this connection
    free(chdata->conn);
    free(chdata);
    // close resources associated with the connection
    pthread_exit(NULL);
}

void *async_listen_func(void *data) {
    socket_server *srv = (socket_server *)data;
    pthread_t thread;
    for (;;) {
        // detach the thread as we aren't join to join with it
        // pthread_detach(thread);
        if (_do_exit == true) {
            // break out of loop trigger closure
            break;
        }
        client_conn *conn = accept_client_conn(srv);
        if (conn == NULL) {
            sleep(0.50); // sleep for 500 miliseconds
            continue;
        }
        wait_group_add(srv->wg, 1);
        /*pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);*/
        conn_handle_data *chdata = malloc(sizeof(conn_handle_data));
        chdata->srv = srv;
        chdata->conn = conn;
        chdata->thread = thread;
        pthread_create(&thread, NULL, async_handle_conn_func, chdata);
        // here we detach, in theory we can use join
        pthread_detach(thread);
    }
    wait_group_done(srv->wg);
    pthread_exit(NULL);
}

client_conn *accept_client_conn(socket_server *srv) {
    // temporary variable for storing socket address
    sock_addr_storage addr_temp;
    // set client_len
    // i tried doing `(sock_addr *)&sizeof(addr_temp)
    // in the `accept` function call but it didnt work
    socklen_t client_len = sizeof(addr_temp);
    int client_socket_num = accept(
        srv->socket_number,
        (sock_addr *)&addr_temp,
        &client_len
    );
    // socket number less than 0 is an error
    if (client_socket_num < 0) {
        return NULL;
    }
    client_conn *connection = malloc(sizeof(client_conn));
    if (connection == NULL) {
        return NULL;
    }
    sock_addr_storage addr = addr_temp;
    connection->address = &addr;
    connection->socket_number = client_socket_num;
    char *addr_info = get_name_info((sock_addr *)connection->address);
    srv->logf(srv->thl, 0, LOG_LEVELS_INFO, "accepted new connection: %s", addr_info);
    free(addr_info);
    return connection;
}

char  *get_name_info(sock_addr *client_address) {
    char address_info[256]; // destroy when function returns
    getnameinfo(
        client_address,
        sizeof(*client_address),
        address_info, // output buffer
        sizeof(address_info), // size of the output buffer
        0, // second buffer which outputs service name
        0, // length of the second buffer
        NI_NUMERICHOST    // want to see hostnmae as an ip address
    );
    char *addr = malloc(sizeof(address_info));
    if (addr == NULL) {
        return NULL;
    }
    strcpy(addr, address_info);
    return addr;
}

addr_info default_hints() {
    addr_info hints;
    memset(&hints, 0, sizeof(hints));
    // change to AF_INET6 to use IPv6
    hints.ai_family = AF_INET;
    // indicates TCP, if you want UDP use SOCKT_DGRAM
    hints.ai_socktype = SOCK_STREAM;
    // indicates to getaddrinfo we want to bind to the wildcard address
    hints.ai_flags = AI_PASSIVE;
    return hints;
}

/*
    this likely has memory leaks and needs proper post-cleanup
*/
socket_server *new_socket_server(addr_info hints, thread_logger *thl, int max_conns, char *port) {
    addr_info *bind_address;
    // using the information contained in hints
    // get address data for this socket and store it in bind_address
    int rc = getaddrinfo(0, port, &hints, &bind_address);
    if (rc != 0) {
        thl->log(thl, 0, "failed to getaddrinfo", LOG_LEVELS_ERROR);
        return NULL;
    }
    // creates the socket and gets us its file descriptor
    int listen_socket_num = socket(
        bind_address->ai_family,
        bind_address->ai_socktype,
        bind_address->ai_protocol
    );
    int one = 1;
    // set socket options before doing anything else
    // i tried setting it after listen, but I don't think that works
    rc = setsockopt(listen_socket_num, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(int));
    if (rc != 0) {
        printf("failed to setsockopt\n");
        return NULL;
    }
    // less than 0 is an error
    if (listen_socket_num < 0) {
        thl->log(thl, 0, "socket creation failed", LOG_LEVELS_ERROR);
        return NULL;
    }
    // binds the address to the socket
    bind(
        listen_socket_num,
        bind_address->ai_addr,
        bind_address->ai_addrlen
    );
    if (errno != 0) {
        thl->logf(thl, 0, LOG_LEVELS_ERROR, "socket bind failed with error %s", strerror(errno));
        return NULL;
    }
    // free up addrinfo resources
    freeaddrinfo(bind_address);
    // start listening on the socket and begin accepting connection
    listen(listen_socket_num, max_conns);
    if (errno != 0) {
        thl->logf(thl, 0, LOG_LEVELS_ERROR, "failed to start listening on socket with error %s", strerror(errno));
        return NULL;
    }
    socket_server *srv = calloc(sizeof(socket_server), sizeof(socket_server));
    if (srv == NULL) {
        thl->log(thl, 0,"socket_srv malloc failed", LOG_LEVELS_ERROR);
        return NULL;
    }
    wait_group_t *wg = wait_group_new();
    if (wg == NULL) {
        srv->log(thl, 0, "failed to initialize wait_group_t", LOG_LEVELS_ERROR);
        return NULL;
    }
    srv->wg = wg;
    srv->socket_number = listen_socket_num;
    srv->thl = thl;
    // the following two functions are shorthands for thl->log[f]
    srv->log = thl->log;
    srv->logf = thl->logf;
    bool passed = set_socket_blocking_status(srv->socket_number, false);
    if (passed == false) {
        printf("failed to set socket blocking mode\n");
        return NULL;
    }
    srv->log(thl, 0, "socket server creation succeeded", LOG_LEVELS_INFO);
    return srv;
}

void setup_signal_handling() {
    _do_exit = false; // initialize to false
    // initialize the mutex we use to block access to the boolean global
    pthread_mutex_init(&_signal_mutex, NULL);
    // register signal handling so we can invoke program exit
    signal(SIGINT, signal_handler_fn); // CTRL+C
    signal(SIGTERM, signal_handler_fn);
    signal(SIGQUIT, signal_handler_fn);
}

int main(void) {
    setup_signal_handling();
    thread_logger *thl = new_thread_logger(false);
    if (thl == NULL) {
        printf("new_thread_logger failed\n");
        return -1;
    }
    addr_info hints = default_hints();
    socket_server *srv = new_socket_server(hints, thl, 100, "8081");
    if (srv == NULL) {
        srv->log(thl, 0, "socket server creation failed", LOG_LEVELS_ERROR);
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