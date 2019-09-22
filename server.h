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

#endif
