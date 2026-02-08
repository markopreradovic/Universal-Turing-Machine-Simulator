#ifndef TAPE_H
#define TAPE_H

#include <stdbool.h>
//Tape implemented as a double linked list
typedef char Symbol;

//Single cell from the tape
typedef struct TapeCell {
    Symbol symbol;
    struct TapeCell *left;
    struct TapeCell *right;
} TapeCell;

//Main structure of the tape
typedef struct {
    TapeCell* head;
    Symbol blank;
} Tape;

//Functions
Tape* tape_create(const char* initial_content, Symbol blank);
void tape_destroy(Tape* tape);

Symbol tape_read(const Tape* tape);
void tape_write(Tape* tape, Symbol symbol);
void tape_move_right(Tape* tape);
void tape_move_left(Tape* tape);

void tape_print(const Tape* tape, int visible_cells);

#endif