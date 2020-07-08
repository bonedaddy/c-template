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

#define ISVALIDSOCKET(s) ((s) >= 0)
#define CLOSESOCKET(s) close(s)
#define SOCKET int
#define GETSOCKETERRNO() (errno)

void print_and_exit(int error_number);

void print_and_exit(int error_number) {
    printf("failure detected: %s\n", strerror(error_number));
    exit(-1);
}

int main(void) {
    struct addrinfo hints;
    struct addrinfo *bind_address;
    int listen_socket_num;
    // sets hints to all 0
    memset(&hints, 0, sizeof(hints));
    // change to AF_INET6 to use IPv6
    hints.ai_family = AF_INET;
    // indicates TCP, if you want UDP use SOCKT_DGRAM
    hints.ai_socktype = SOCK_STREAM;
    // indicates to getaddrinfo we want to bind to the wildcard address
    hints.ai_flags = AI_PASSIVE;
    // hints allows speicfying the information we are looking for
    getaddrinfo(0, "8081", &hints, &bind_address);
    // generate the actual socket
    listen_socket_num = socket(
        bind_address->ai_family,
        bind_address->ai_socktype,
        bind_address->ai_protocol    
    );
    if (listen_socket_num < 0) { // could also check against errno
        printf("failed to create socket\n");
        return -1;
    }
    bind(
        listen_socket_num, 
        bind_address->ai_addr, 
        bind_address->ai_addrlen
    );
    if (errno != 0) {
        print_and_exit(errno);
    }
    // free resources allocated to bind_address
    freeaddrinfo(bind_address);
    // start accepting up to N connections
    listen(listen_socket_num, /*max cons*/ 100);
    if (errno != 0) {
        print_and_exit(errno);
    }
    // stores the address info for the connecting client
    struct sockaddr_storage client_address;
    socklen_t client_len = sizeof(client_address);
    // blocks until a new connection is made
    // and when connection is made we create a new socket for it
    // the returned socket number will be usable to send and receive data over
    int client_socket_num = accept(
        listen_socket_num,
        (struct sockaddr*)&client_address,
        &client_len // specifies the size of the buffer we're passing in
    );
    if (client_socket_num < 0) {
        printf("failed to create client socket\n");
        return -1;
    }
    char address_buffer[100];
    getnameinfo(
        (struct sockaddr*)&client_address, 
        client_len, 
        address_buffer, // output buffer
        sizeof(address_buffer), // size of the output buffer
        0, // second buffer which outputs service name
        0, // length of the second buffer
        NI_NUMERICHOST    // want to see hostnmae as an ip address
    );
    printf("%s\n", address_buffer);
    close(listen_socket_num);
    close(client_socket_num);
} 