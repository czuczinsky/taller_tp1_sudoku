#define _POSIX_C_SOURCE 200112L
#include "socket.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h> // bool
#include <errno.h> //erno
#include <sys/socket.h> // socket(), connect()
#include <unistd.h> // close()
#include <netdb.h> // getaddrinfo

void socket_create(socket_t* self){
    self->fd = -1;
}

void socket_destroy(socket_t* self){
    if (self->fd != -1){
        shutdown(self->fd,2);
        close(self->fd);
        self->fd = -1;
    }
}

int socket_connect(socket_t* self, const char* host, const char* service){
    struct addrinfo hints;
    struct addrinfo *ai_list, *ptr;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;

    if(getaddrinfo(host, service, &hints, &ai_list) != 0){
        perror("GETADDRINFO: ");
        return -1; 
    }

    bool are_we_connected = false;
    int s = 0;
    for (ptr = ai_list; ptr != NULL && are_we_connected == false; ptr = ptr->ai_next) {
        self->fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if(self->fd == -1){
            perror("SOCKET: ");
        } else {
            s = connect(self->fd,ptr->ai_addr, ptr->ai_addrlen);
            if(s != 0){
                perror("CONNECT: ");
            }
            are_we_connected = (s != -1);
        }
    }
    
    freeaddrinfo(ai_list);

    if(self->fd == -1){
        return -1;
    }
    return 0;
}

int socket_bind_and_listen(socket_t *self, const char *service) {
    struct addrinfo hints;
    struct addrinfo *ai_list, *ptr;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if(getaddrinfo(NULL, service, &hints, &ai_list) !=0 ){
        perror("GETADDRINFO: ");
        return -1;
    }
    
    bool are_we_connected = false;
    int bind_status = 0;
    for (ptr = ai_list; ptr != NULL && are_we_connected == false; ptr = ptr->ai_next) {
        self->fd = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
        if(self->fd == -1){
            perror("SOCKET: ");
        } else {
            bind_status = bind(self->fd,ptr->ai_addr, ptr->ai_addrlen);
            if(bind_status == -1){
                perror("BIND: ");
            }
            are_we_connected = (bind_status != -1);
        }
    }
    
    freeaddrinfo(ai_list);

    if(listen(self->fd,1)==-1){
        perror("LISTEN: ");
        return -1;
    }

    return 0;
}

int socket_accept(socket_t *self, socket_t *new_socket){
    self->fd = accept(self->fd, NULL, NULL);
    if(self->fd==-1){
        perror("ACCEPT: ");
        return -1;
    }
    return 0;
}

int socket_send(socket_t *self, char *buffer, size_t length){
    int bytes_sent = 0;
    ssize_t status = 0;
    while (bytes_sent < length && status != -1) {
        status = send(self->fd, &buffer[bytes_sent], length - bytes_sent, MSG_NOSIGNAL);
        if (status == 0) {
            return -1;
        } else if (status < 0) {
            perror("RECV: ");
        } else {
            bytes_sent += status;
        }
    }
    return bytes_sent;
}

int socket_recv(socket_t *self, char *buffer, size_t length){
    int bytes_recv = 0;
    ssize_t status = 0;
    while (bytes_recv < length) {
        status = recv(self->fd, &buffer[bytes_recv], length - bytes_recv, 0);
        if (status == 0) {
            return -1;
        } else if (status < 0){
            perror("RECV: ");
            return -1;
        } else {
            bytes_recv += status;
        }
    }
    return bytes_recv;
}