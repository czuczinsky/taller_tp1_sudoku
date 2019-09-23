#ifndef __SERVER_H__
#define __SERVER_H__
#include "socket.h"
#include "sudoku.h"

typedef struct {
    socket_t socket;
    socket_t sudoku_socket;
    sudoku_t sudoku;
} server_t;

void server_open(server_t *self,const char *service);

void server_close(server_t *self);

void server_run(server_t *self);

void __server_send_board(server_t *self);

void __server_send_msg(server_t *self, int length, char *msg);

void __server_verify(server_t *self);

int __server_put(server_t *self);

void __server_reset(server_t *self);

#endif
