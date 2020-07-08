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
#include <time.h>
#include <stdbool.h>
#include <pthread.h>
#include "../../include/utils/logger.h"

typedef struct addrinfo addr_info;
typedef struct sockaddr sock_addr;
typedef struct sockaddr_storage sock_addr_storage;

typedef struct socket_server {
    int socket_number;
    log_fn log; // alis for thl->log
    log_fnf logf; // alias for thl->logf
    thread_logger *thl;
    pthread_t thread;
    pthread_attr_t taddr;
} socket_server;

typedef struct client_conn {
    int socket_number;
    sock_addr_storage *address;
} client_conn;

addr_info default_hints();
socket_server *new_socket_server(addr_info hints, thread_logger *thl, int max_conns, char *port);
client_conn *accept_client_conn(socket_server *srv);

char  *get_name_info(sock_addr *client_address);
void *async_handle_conn_func(void *data);
void *async_listen_func(void *data);
void print_and_exit(int error_number);
void close_client_conn(client_conn *conn);
void close_socket_server(socket_server *srv);

void close_client_conn(client_conn *conn) {
    close(conn->socket_number);
}
void close_socket_server(socket_server *srv) {
    close(srv->socket_number);
    clear_thread_logger(srv->thl);
    free(srv);
}

void print_and_exit(int error_number) {
    printf("failure detected: %s\n", strerror(error_number));
    exit(-1);
}

typedef struct conn_handle_data {
    pthread_t thread;
    socket_server *srv;
    client_conn *conn;
} conn_handle_data;

void *async_handle_conn_func(void *data) {
    conn_handle_data *chdata = (conn_handle_data *)data;
    char *address_buffer = get_name_info((sock_addr *)chdata->conn->address);
    chdata->srv->logf(chdata->srv->thl, 0, LOG_LEVELS_INFO, "new client connected: %s", address_buffer);
    char request[1024];
    for (;;) {
        int bytes_received = recv(
            chdata->conn->socket_number, 
            request, // output buffer
            1024,  // output buffer size 
            0 // speciifes flags
        );
        if (bytes_received == 0 || bytes_received == -1) {
            chdata->srv->log(chdata->srv->thl, 0, "client disconnected", LOG_LEVELS_INFO);
            break;
        }
        int bytes_sent = send(
            chdata->conn->socket_number,
            request,
            strlen(request),
            0 // specifies flags
        );
        if ((size_t)bytes_sent < strlen(request)) {
            chdata->srv->log(chdata->srv->thl, 0, "sent less bytes than expected", LOG_LEVELS_WARN);
        }
    }
    // close resources associated with the connection
    close_client_conn(chdata->conn);
    // free up data allocated for chdata
    free(chdata);
    return NULL;
}

void *async_listen_func(void *data) {
    socket_server *srv = (socket_server *)data;
    for (;;) {
        client_conn *conn = accept_client_conn(srv);
        if (conn == NULL) {
            srv->log(srv->thl, 0, "failed to accept client connection", LOG_LEVELS_ERROR);
            continue;
        }
        pthread_t thread;
        conn_handle_data *chdata = malloc(sizeof(conn_handle_data));
        chdata->srv = srv;
        chdata->conn = conn;
        chdata->thread = thread;
        pthread_create(&thread, NULL, async_handle_conn_func, chdata);
    }
    return NULL;
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
    // allocate memory for returning the client address
    sock_addr_storage *client_address = malloc(sizeof(addr_temp));
    if (client_address == NULL) {
        return NULL;
    }
    // copy the addr_temp variable data
    client_address = &addr_temp;
    // allocate memory for client_conn struct
    client_conn *connection = malloc(sizeof(client_conn));
    if (client_address == NULL) {
        return NULL;
    }
    connection->address = client_address;
    connection->socket_number = client_socket_num;
    return connection;
}

char  *get_name_info(sock_addr *client_address) {
    char address_info[256];
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
    }
    srv->socket_number = listen_socket_num;
    srv->thl = thl;
    // the following two functions are shorthands for thl->log[f]
    srv->log = thl->log;
    srv->logf = thl->logf;
    // intialize pthread attributes
    pthread_attr_init(&srv->taddr);
    srv->log(thl, 0, "socket server creation succeeded", LOG_LEVELS_INFO);
    return srv;
}

int main(void) {
    thread_logger *thl = new_thread_logger(false);
    addr_info hints = default_hints();
    socket_server *srv = new_socket_server(hints, thl, 100, "8081");
    if (srv == NULL) {
        srv->log(thl, 0, "socket server creation failed", LOG_LEVELS_ERROR);
        return -1;
    }
    pthread_create(
        &srv->thread,
        &srv->taddr,
        async_listen_func,
        srv
    );
    for (;;) { }
    // close allocated sockets
    close_socket_server(srv);
    // close_client_conn(conn);
} 