#include "cell.h"

void cell_init(cell_t* self, int value){
    if (value == '0') {
        self->modifiable = 0; 
    } else {
        self->modifiable = 1;
    }
    self->reset_value = value;
    self->value = value;
}

int cell_put(cell_t* self, int value){
    if (self->modifiable == 0) {
        self->value = value;
        return 0;
    } else {
        return -1;
    }
}

int cell_get(cell_t* self){
    return self->value;
}

void cell_reset(cell_t* self){
    self->value = self->reset_value;
}


