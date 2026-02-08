// src/main.c

#include "tape.h"
#include <stdio.h>

int main(void) {
    Tape* t = tape_create("010101", '_');

    printf("Početno stanje:\n");
    tape_print(t, 10);

    tape_move_right(t);
    tape_write(t, 'X');
    tape_print(t, 10);

    tape_move_left(t);
    tape_move_left(t);
    tape_move_left(t);
    tape_write(t, 'Y');
    tape_print(t, 10);

    tape_destroy(t);
    return 0;
}