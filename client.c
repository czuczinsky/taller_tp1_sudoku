#include "client.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

void client_open(client_t* self, char* host, char* service){
    socket_connect(&self->socket,host,service);
}

void client_close(client_t* self){
    socket_destroy(&self->socket);
}

void client_handle(client_t *self){
    char input[13];
    do {
        if (fgets(input,sizeof(input),stdin)==NULL) {
            memset(input,0,sizeof(input));
            strncpy(input,"exit",4);
        }
        memset(self->buffer,0,sizeof(self->buffer));
        if (strncmp("get",input,3)==0) {
            __client_get(self);
        } else if (strncmp("verify",input,6)==0) {
            __client_verify(self);
        } else if (strncmp("put",input,3)==0) {
            __client_put(self, input);
        } else if (strncmp("reset",input,3)==0) {
            __client_reset(self);
        }
        printf("%s\n", self->buffer);
    } while (strncmp("exit",input,4)!=0);
}

void __client_command(client_t *self, char *cmd, int len_cmd) {
    uint32_t len_buff;
    socket_send(&self->socket, cmd, len_cmd);
    // Los primero 4 bytes son el largo del msj a recibir
    socket_recv(&self->socket, (char *) &len_buff, 4);
    // length_buffer_recive = ntohl(length_buffer_recive);
    // Reconfiguro a los endianes del sys
    socket_recv(&self->socket, self->buffer, len_buff);
    self->buffer[len_buff]=0;
}

void __client_get(client_t *self) {
    __client_command(self, "G", 1);
}

void __client_verify(client_t *self) {
    __client_command(self, "V", 1);
}

void __client_reset(client_t *self) {
    __client_command(self, "R", 1);
}

void __client_put(client_t *self, char *input) {
    uint8_t value, row, column;
    sscanf(input,"%*s %d %*s %d %*c %d",
            (int *)&value,(int *)&row,(int *)&column);
    // TODO modificar los endianes
    if (value <= 9 && value >=1 &&
        row <= 9 && row >=1 &&
        column <= 9 && value >=1 ) {
        char send_buffer[5];
        memset(send_buffer,0,5);
        strncat(send_buffer,"P",1);
        strncat(send_buffer,(char *)&row,1);
        strncat(send_buffer,(char *)&column,1);
        strncat(send_buffer,(char *)&value,1);
        __client_command(self, send_buffer, 4);
        //return 0;
    }
   // return -1;
}
