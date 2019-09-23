#include "server.h"
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include "sudoku.h"

void server_open(server_t *self, const char *service) {
    socket_t sudoku_socket;
    socket_create(&self->socket);
    socket_bind_and_listen(&self->socket, service);
    socket_accept(&self->socket, &sudoku_socket);
    sudoku_create(&self->sudoku);
}

void server_close(server_t *self) {
    sudoku_destroy(&self->sudoku);
    socket_destroy(&self->socket);
}

void server_run(server_t *self) {
    char command_letter[1];
    while (socket_recv(&self->socket, command_letter, 1) != -1) {
        if (strncmp("P",command_letter,1)==0) {
            __server_put(self);
            __server_send_board(self);
        } else if (strncmp("G",command_letter,1)==0) {
            __server_send_board(self);
        } else if (strncmp("V",command_letter,1)==0) {
            __server_verify(self);
        } else if (strncmp("R",command_letter,1)==0) {
            __server_reset(self);
        }
        memset(command_letter,0,sizeof(command_letter));
    }
}

void __server_send_board(server_t *self) {
    int board_buffer_length = 722;
    char board_buffer[722];
    memset(board_buffer,0,sizeof(board_buffer));
    sudoku_get(&self->sudoku, board_buffer);
    socket_send(&self->socket,(char *)&board_buffer_length, 4);
    socket_send(&self->socket,board_buffer,sizeof(board_buffer));
}

void __server_put(server_t *self) {
    char recive_buffer[3];
    socket_recv(&self->socket, recive_buffer, sizeof(recive_buffer));
    uint8_t row = recive_buffer[0];
    uint8_t column = recive_buffer[1];
    uint8_t value = recive_buffer[2];
    value = value + 48; // Sumo offset del ASCII
    sudoku_put(&self->sudoku, value, row, column);
}

void __server_verify(server_t *self) {
    int msg_length;
    if (sudoku_verify(&self->sudoku) == 0){
        msg_length = 2;
        __server_send_msg(self, msg_length, "Ok");
    } else {
        msg_length = 5;
        __server_send_msg(self, msg_length, "Error");
    }
}

void __server_send_msg(server_t *self, int length, char *msg) {
    socket_send(&self->socket, (char *) &length, 4);
    socket_send(&self->socket, msg, length);
}

void __server_reset(server_t *self) {
    sudoku_reset(&self->sudoku);
    __server_send_board(self);
}
