#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__
#include "socket.h"

void _protocol_command(char *command, socket_t *socket, int byte_length_command, char *buffer);

void protocol_get(socket_t *socket, char *buffer);

void protocol_put(socket_t *socket, char *input, char *buffer);

void protocol_verify(socket_t *socket, char *buffer);

void protocol_reset(socket_t *socket, char *buffer);

#endif