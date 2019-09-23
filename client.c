#include "client.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <arpa/inet.h>

void client_open(client_t* self, char* host, char* service){
    socket_connect(&self->socket,host,service);
}

void client_close(client_t* self){
    socket_destroy(&self->socket);
}

void client_handle(client_t *self){
    char input[20];
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
    } while (strncmp("exit",input,4)!=0);
}

void __client_command(client_t *self, char *cmd, int len_cmd) {
    uint32_t len_buff;
    socket_send(&self->socket, cmd, len_cmd);
    // Los primero 4 bytes son el largo del msj a recibir
    socket_recv(&self->socket, (char *) &len_buff, 4);
    // Reconfiguro a los endianes del sys
    len_buff = ntohl(len_buff);
    socket_recv(&self->socket, self->buffer, len_buff);
    self->buffer[len_buff]=0;
}

void __client_get(client_t *self) {
    __client_command(self, "G", 1);
    printf("%s\n", self->buffer);
}

void __client_verify(client_t *self) {
    __client_command(self, "V", 1);
    if (strncmp(self->buffer, "Ok\n", 3) == 0) {
        printf("%s\n", self->buffer);
    } else {
        char msgErr[] = 
            "ERROR\n";
        fprintf(stderr, "%s", msgErr);
    }
}

void __client_reset(client_t *self) {
    __client_command(self, "R", 1);
    printf("%s\n", self->buffer);
}

void __client_put(client_t *self, char *input) {
    int min = 1;
    int max = 9;
    uint8_t value, row, column;
    sscanf(input,"%*s %d %*s %d %*c %d",
            (int *)&value,(int *)&row,(int *)&column);
    if (__client_value_in_range(row, min, max) &&
        __client_value_in_range(column, min, max)) {
        if (__client_value_in_range(value, min, max)) {
            char send_buffer[5];
            memset(send_buffer,0,5);
            strncat(send_buffer,"P",1);
            strncat(send_buffer,(char *)&row,1);
            strncat(send_buffer,(char *)&column,1);
            strncat(send_buffer,(char *)&value,1);
            __client_command(self, send_buffer, 4);
            if (__client_no_mod_cell(self)) {
                char msgErr[] = 
                    "La celda indicada no es modificable\n";
                fprintf(stderr, "%s", msgErr);
            } else {
                printf("%s\n", self->buffer);
            }
        } else {
            char msgErr[] = 
                "Error en el valor ingresado. Rango soportado: [1,9]\n";
            fprintf(stderr, "%s", msgErr);
        }
    } else {
        char msgErr[] = 
            "Error en los indices. Rango soportado: [1,9]\n";
        fprintf(stderr, "%s", msgErr);
    }
}

bool __client_value_in_range(int value, int min, int max) {
    if (value <= max && value >= min) {
        return true;
    } else {
        return false;
    }
}

bool __client_no_mod_cell(client_t *self) {
    if (strncmp(self->buffer, "nmc", 3) == 0) {
        return true;
    } else {
        return false;
    }
}
