#include "cell.h"

void cell_init(cell_t* self, int value){
    self->reset_value = value;
    cell_put(self, value);
}

void cell_put(cell_t* self, int value){
    self->value = value;
}

int cell_get(cell_t* self){
    return self->value;
}

void cell_reset(cell_t* self){
    self->value = self->reset_value;
}


