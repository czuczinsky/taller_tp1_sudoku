#ifndef __SUDOKU_H__
#define __SUDOKU_H__
#include "cell.h"

typedef struct{
    cell_t *board;
} sudoku_t;

void sudoku_create(sudoku_t *self);

int sudoku_put(sudoku_t *self, int value, int row, int column);

int sudoku_verify(sudoku_t *self);

void sudoku_reset(sudoku_t *self);

void sudoku_destroy(sudoku_t *self);

int sudoku_fill_board(sudoku_t *self);

void sudoku_get(sudoku_t *self, char *buffer);

int sudoku_rows_verify(sudoku_t *self);

int sudoku_columns_verify(sudoku_t *self);

int sudoku_regions_verify(sudoku_t *self);

#endif
