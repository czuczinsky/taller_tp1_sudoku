#include "protocol.h"
#include "socket.h"
#include <stdint.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>

void _protocol_command(char *command, socket_t *socket, int byte_length_command, char *buffer){
    uint32_t length_buffer_recive;
    socket_send(socket, command, byte_length_command);
    socket_recv(socket, (char *) &length_buffer_recive, 4); // Los primero 4 bytes son el largo del msj a recibir
    //length_buffer_recive = ntohl(length_buffer_recive); // Reconfiguro a los endianes del sys
    socket_recv(socket, buffer, length_buffer_recive);
    buffer[length_buffer_recive]=0;
}

void protocol_get(socket_t *socket, char *buffer) {
    _protocol_command("G",socket, 1, buffer);
}

void protocol_verify(socket_t *socket, char *buffer) {
    _protocol_command("V", socket, 1, buffer);
}

void protocol_put(socket_t *socket, char *input, char *buffer) {
    uint8_t value, row, column;
    sscanf(input,"%*s %d %*s %d %*c %d",(int *)&value,(int *)&row,(int *)&column);
    // TODO modificar los endianes
    if (value <= 9 && value >=1 &&
        row <= 9 && row >=1 &&
        column <= 9 && value >=1 ) {
        char send_buffer[4] = "P";
        strncat(send_buffer,(char *)&row,1);
        strncat(send_buffer,(char *)&column,1);
        strncat(send_buffer,(char *)&value,1);
        _protocol_command(send_buffer, socket, 4, buffer);
        //return 0;
    }
   // return -1;
}

void protocol_reset(socket_t *socket, char *buffer) {
    _protocol_command("R", socket, 1, buffer);
}