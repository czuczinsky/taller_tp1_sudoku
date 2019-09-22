#include "server.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "sudoku.h"

void server_open(server_t *self, const char *service){
    socket_t sudoku_socket;
    socket_create(&self->socket);
    socket_bind_and_listen(&self->socket, service);
    socket_accept(&self->socket, &sudoku_socket);
    sudoku_create(&self->sudoku);
}

void server_close(server_t *self){
    sudoku_destroy(&self->sudoku);
    socket_destroy(&self->socket);
}

void server_run(server_t *self){
    char command_letter[1];
    while(socket_recv(&self->socket, command_letter, 1) != -1){
        if (strncmp("P",command_letter,1)==0){
            char recive_buffer[3];
            socket_recv(&self->socket, recive_buffer, sizeof(recive_buffer));
            uint8_t row = recive_buffer[0];
            uint8_t column = recive_buffer[1];
            uint8_t value = recive_buffer[2];
            sudoku_put(&self->sudoku, value, row, column);
        } else if (strncmp("G",command_letter,1)==0) {
            int board_buffer_length = 722;
            char board_buffer[board_buffer_length];
            memset(board_buffer,0,sizeof(board_buffer));
            sudoku_get(&self->sudoku, board_buffer);
            socket_send(&self->socket,(char *)&board_buffer_length, 4);
            socket_send(&self->socket,board_buffer,sizeof(board_buffer));
        } else if (strncmp("V",command_letter,1)==0) {
            if (sudoku_verify(&self->sudoku) == -1){
                int msg_length = 5;
                socket_send(&self->socket, (char *) &msg_length, 4);
                socket_send(&self->socket, "Error", msg_length);
            } else {
                // TODO "ok"
            }
        } else if (strncmp("R",command_letter,1)==0) {
            printf("Se resetea\n");
        }
        memset(command_letter,0,sizeof(command_letter));
    }
}