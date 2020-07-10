/*! @file socket_server.c
  * @author Bonedaddy
  * @brief TCP socket servers, clients, and tooling for working with sockets
  * @details uses wait_group.h to provide lightweight synchronization between pthreads
  * @warning before use you must call setup_signal_handling() so that all threads get properly cleaned up on exit
  * @note you will want to adjust `async_handle_conn_func` to suit your needs as right now it is just an echo client
  * it is likely you will need to have `#define _POSIX_C_SOURCE 201112L` 
  * see the following for more information
  *    - https://stackoverflow.com/questions/39409846/why-does-gcc-not-complain-about-htons-but-complains-about-getaddrinfo-when-c/39410095#39410095
  *    - https://man7.org/linux/man-pages/man3/getaddrinfo.3.html
*/


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
#include <stdbool.h>
#include <pthread.h>
#include <signal.h>
#include <fcntl.h>
// sys/time.h is needed for the timeval
//  #include <time.h>
#include <sys/time.h>
#include "../../../include/utils/logger.h"
#include "../../../include/sync/wait_group.h"
#include "../../../include/network/socket/socket.h"
#include "../../../include/network/socket/socket_server.h"

/*! @brief returns a new socket server bound to the port number and ready to accept connections
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
    SOCKET_OPTS opts[2] = {REUSEADDR, NOBLOCK};
    int listen_socket_num = get_new_socket(
        thl,
        bind_address,
        opts,
        2
    );
    if (listen_socket_num == -1) {
        thl->log(thl, 0, "failed to get_new_socket", LOG_LEVELS_ERROR);
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
    srv->log(thl, 0, "socket server creation succeeded", LOG_LEVELS_INFO);
    return srv;
}


/*! @brief listens for new connections and spawns a thread to process the connection
  * thread that is created to process the connection runs as a detached thread
  * will poll for new connections to accept every 500 miliseconds
  * @param data void pointer to a socket_server struct
  * @note detached thread created calling async_handle_conn_func
  * @warning may change the 500 milisecond sleep
*/
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

/*! @brief handles connections in a dedicated pthread   
  * is laucnched in a pthread by async_listen_func when any new connection is received
  * @param data `void *` to a conn_handle_data object
  * @note uses `select` to determine if we can read data from the connection
  * @note select runs for 3 seconds before timing out and releasing resources with the connection
  * @warning currently implements an example echo client
  * @warning you will want to adapt to your specific use case
*/
void *async_handle_conn_func(void *data) {
    conn_handle_data *chdata = (conn_handle_data *)data;
    fd_set socket_set;
    FD_ZERO(&socket_set); // always zero out before use
    FD_SET(chdata->conn->socket_number, &socket_set);
    // FD_CLR() // used to remove from set
    // must provide a number larger than last socket descriptor
    int max_socket = chdata->conn->socket_number + 1;
    // copy since select() modifies stuff
    // not needed in our case since this is for a dedicated conn
    // fd_set socket_set_copy;
    // socket_set_copy = socket_set;
    // set a timeout of 3 seconds
    struct timeval timeout;
    timeout.tv_sec = 3;
    timeout.tv_usec = 0;
    int rc = select(
        max_socket, 
        &socket_set, 
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
            if (FD_ISSET(chdata->conn->socket_number, &socket_set)) {
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

/*! @brief helper function for accepting client connections
  * times out new attempts if they take 3 seconds or more
  * @return Failure: NULL client conn failed
  * @return Success: non-NULL populated client_conn object
*/
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
    connection->address = &addr_temp;
    connection->socket_number = client_socket_num;
    char *addr_info = get_name_info((sock_addr *)connection->address);
    srv->logf(srv->thl, 0, LOG_LEVELS_INFO, "accepted new connection: %s", addr_info);
    free(addr_info);
    return connection;
}