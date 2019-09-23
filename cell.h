#ifndef __CELL_H__
#define __CELL_H__

typedef struct {
    char value;
    char reset_value;
    char modifiable;
} cell_t;

void cell_init(cell_t *self, int value);

int cell_put(cell_t *self, int value);

int cell_get(cell_t *self);

void cell_reset(cell_t *self);

#endif
