#include "client.h"
#include "protocol.h"
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
    do{
        if(fgets(input,sizeof(input),stdin)==NULL){
            memset(input,0,sizeof(input));
            strncpy(input,"exit",4);
        }
        memset(self->buffer,0,sizeof(self->buffer));
        if(strncmp("get",input,3)==0){
            protocol_get(&self->socket, self->buffer);
        } else if(strncmp("verify",input,6)==0){
            protocol_verify(&self->socket, self->buffer);
        } else if(strncmp("put",input,3)==0){
            protocol_put(&self->socket, input, self->buffer);
        } else if(strncmp("reset",input,3)==0){
            protocol_reset(&self->socket, self->buffer);
        }
        printf("%s\n", self->buffer);
    }while(strncmp("exit",input,4)!=0);
}


