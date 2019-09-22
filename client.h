#ifndef __CLIENT_H__
#define __CLIENT_H__
#include "socket.h"

typedef struct{
    socket_t socket;
    char buffer[723];
} client_t;

void client_open(client_t *self, char *host, char *service);

void client_close(client_t *self);

void client_handle(client_t *self);

#endif
