/*! @file socket.h
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

#pragma once

/*! @def _POSIX_C_SOURCE 201112L
  * @brief fixes GCC not compiling correctly
  * currently we are handling this via our cmake build system (see CMakeLists.txt)
  * however you may want to define the macro in source yourself
  * @note https://stackoverflow.com/questions/39409846/why-does-gcc-not-complain-about-htons-but-complains-about-getaddrinfo-when-c/39410095#39410095
*/

#include <pthread.h>
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
#include "../../utils/logger.h"


/*! @brief used to lock writes for _do_exit
  * @warning should not be called libraries using socket.c
*/
pthread_mutex_t _signal_mutex;
/*! @brief used to indicate when threads need to cleanup
  * @warning should not be called libraries using socket.c
  * polled at the beginning of every async_listen_func loop and async_handle_conn_func
  * when set to true causes active pthreads to exit after successfully cleaning up
*/
bool _do_exit;

/*! @typedef addr_info
  * @struct addrinfo
  * @brief alias for `struct addrinfo`
*/
typedef struct addrinfo addr_info;

/*! @typedef sock_addr
  * @struct sockaddr
  * @brief alias for `struct sockaddr`
*/
typedef struct sockaddr sock_addr;

/*! @typedef sock_addr_storage
  * @struct sockaddr_storage
  * @brief alias for `struct sockaddr_storage`
*/
typedef struct sockaddr_storage sock_addr_storage;

/*! @typedef process_fd_fn
  * @struct process_fd_fn
  * @brief function to invoke when iterating over an fd_set
*/
typedef void (*process_fd_fn)(int fd_num);

/*! @enum SOCKET_OPTS
  * @brief used to configure new sockets
*/
typedef enum {
  /*! sets socket with SO_REUSEADDR */
  REUSEADDR,
  /*! sets socket to non-blocking mode */
  NOBLOCK,
  /*! sets socket to blocking mode */
  BLOCK,
} SOCKET_OPTS;

/*! @typedef socket_client
  * @struct socket_client
  * a generic tcp socket client
*/
typedef struct socket_client {
    int socket_number;
} socket_client;

/*! @brief returns a new socket client connected to `addr:port`
*/
socket_client *new_socket_client(thread_logger *thl, addr_info hints, char *addr, char *port);

/*! @brief  gets an available socket attached to bind_address
  * @return Success: file descriptor socket number greater than 0
  * @return Failure: -1
  * initializers a socket attached to bind_address with sock_opts, and binds the address
*/
int get_new_socket(thread_logger *thl, addr_info *bind_address, SOCKET_OPTS sock_opts[], int num_opts);

/*! @brief a helper function for `send` logging the failure message
  * @note caller is responsible for freeing up memory allocated for `data` if needed
  * @returns Failure: -1
  * @returns Success: 0
*/
int socket_send(thread_logger *thl, int fd, char *data, int data_size);

/*! @brief a helper function for `recv` logging the failure message
  * @note caller must free up the returned memory when no longer needed
  * @returns Failure; NULL ptr
  * @returns Success: char* of received data
*/
char *socket_recv(thread_logger *thl, int fd);

/*! @brief prepares library for usage
  * @warning must be called before using the library
  * sets up internal mutex, and system signal handling for terminating the server
  * listes to SIGINT, SIGTERM, and SIGQUIT which will terminate the server
*/
void setup_signal_handling();

/*! @brief used to enable/disable blocking sockets
  * @return Failure: false
  * @return Success: true
  * @note see https://stackoverflow.com/questions/1543466/how-do-i-change-a-tcp-socket-to-be-non-blocking/1549344#1549344
*/
bool set_socket_blocking_status(int fd, bool blocking);


/*! @brief callback function used to handle OS signals
  * shouldn't be called directly and instead used as the func in `signal(SIGTERM, handler_fn)`
*/
void signal_handler_fn(int signal_number);

/*! @brief returns the address the client is connecting from
*/
char  *get_name_info(sock_addr *client_address);

/*! @brief generates an addr_info struct with defaults
  * defaults is IPv4, TCP, and AI_PASSIVE flags
*/
addr_info default_hints();


/*! @brief iterates over an fd_set and applies a function to found ones
  * iterates over 0 -> max_socket checking and checks in FD_IS_SET.
  * if set then we run `fn(fd_num)`
  * 
*/
void process_fd_set(fd_set fds, int max_socket, process_fd_fn fn);