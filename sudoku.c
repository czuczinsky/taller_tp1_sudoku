#include "sudoku.h"
#include <stdio.h>
#include <stdlib.h>
#include "cell.h"
#include <errno.h>
#include <string.h>

void sudoku_create(sudoku_t *self){
    self->board = malloc(81 * sizeof (cell_t));
    sudoku_fill_board(self);
}

int sudoku_fill_board(sudoku_t *self){
    FILE* board_file = fopen("board.txt", "r");
    if (board_file == NULL){
        printf("El archivo no existe");
        return -1;
    } else {
        int c;
        int cell_index = 0;
        while ( ( c = getc ( board_file ) ) != EOF ) {
            if ( c != 32 && ( char ) c != *"\n" ) {
                cell_init (&self->board[cell_index],c);
                cell_index++;
            }
        }
        fclose(board_file);
    }
    return 0;
}

void sudoku_put(sudoku_t* self, int value, int row, int column) {
    int cell_index = ((row - 1)*9+(column-1));
    cell_put(&self->board[cell_index],value);
}

void sudoku_reset(sudoku_t *self){
    for(int cell_index = 0; cell_index < 81; cell_index++){
        cell_reset(&self->board[cell_index]);
    }
}

void sudoku_destroy(sudoku_t *self){
    free(self->board);
}

void sudoku_get(sudoku_t* self, char *buffer){
    char high_sep[] = "U===========U===========U===========U\n";
    char low_sep[] = "U---+---+---U---+---+---U---+---+---U\n";
    strncat(buffer,high_sep,38);
    for(int row_index = 0; row_index < 9; row_index++){
        strncat(buffer,"U",1);
        for(int column_index = 0; column_index < 9; column_index++){
            int cell_index = ((row_index*9)+column_index);
            if (cell_get(&self->board[cell_index])!= 48){
                char c_value = (char)cell_get(&self->board[cell_index]);
                strncat(buffer," ",1);
                strncat(buffer, &c_value,1);
                strncat(buffer," ",1);
            } else {
                strncat(buffer,"   ",3);
            }
            if(((column_index+1) % 3) == 0){
                strncat(buffer,"U",1);
            } else {
                strncat(buffer,"|",1);
            }
        }
        strncat(buffer,"\n",1);
        if (row_index != 8) {
            if(((row_index+1) % 3) == 0){
                strncat(buffer,high_sep,38);
            } else {
                strncat(buffer,low_sep,38);
            }
        }
    }
    strncat(buffer,high_sep,37);
}

int sudoku_verify(sudoku_t *self){
    if(sudoku_rows_verify(self) == 0
     && sudoku_columns_verify(self) == 0
     && sudoku_regions_verify(self) == 0){
        return 0;
    }
    return -1;
}

int sudoku_rows_verify(sudoku_t *self){
    for(int row_index = 0; row_index < 9; row_index++){
        for(int column_index_1 = 0; column_index_1 < 8; column_index_1++){
            int cell_index_1 = ((row_index*9)+column_index_1);
            for(int column_index_2 = column_index_1 + 1; column_index_2 < 9; column_index_2++){
                int cell_index_2 = ((row_index*9)+column_index_2);
                if(self->board[cell_index_1].value == self->board[cell_index_2].value){
                    return -1;
                }
            }
        }
    }
    return 0;
}

int sudoku_columns_verify(sudoku_t *self){
    for(int column_index = 0; column_index < 9; column_index++){
        for(int row_index_1 = 0; row_index_1 < 8; row_index_1++){
            int cell_index_1 = ((row_index_1*9)+column_index);
            for(int row_index_2 = row_index_1 + 1; row_index_2 < 9; row_index_2++){
                int cell_index_2 = ((row_index_2*9)+column_index);
                if(self->board[cell_index_1].value == self->board[cell_index_2].value){
                    return -1;
                }
            }
        }
    }
    return 0;
}

int sudoku_regions_verify(sudoku_t *self){
    for(int row_offset = 0; row_offset < 9; row_offset = row_offset + 3){
        for(int column_offset = 0; column_offset < 9; column_offset = column_offset + 3){
            for(int row_index = 0; row_index < 3; row_index++){
                for(int column_index_1 = 0; column_index_1 < 3; column_index_1++){
                    int cell_index_1 = (((row_index+row_offset)*9)+(column_index_1 + column_offset));
                    for(int column_index_2 = 0; column_index_2 < 3; column_index_2++){
                        int cell_index_2 = (((row_index+row_offset)*9)+(column_index_2+column_offset));
                        if(cell_index_1 != cell_index_2 && self->board[cell_index_1].value == self->board[cell_index_2].value){
                            return -1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}