// src/main.c
#include "tape.h"
#include "tm.h"
#include <stdio.h>
#include <string.h>
int main(void) {
    // Test trake (stari dio)
    Tape* t = tape_create("01", '_');
    tape_print(t, 8);

    tape_move(t, 'R');
    tape_write(t, 'X');
    tape_print(t, 8);

    tape_move(t, 'L');
    tape_move(t, 'L');
    tape_move(t, 'S');          // ostaje na mjestu
    tape_write(t, 'Y');
    tape_print(t, 8);

    tape_destroy(t);

    // Test strukture TM
    TuringMachine* tm = tm_create();

    tm_add_transition(tm, "q0", '0', "q1", '1', 'R');
    tm_add_transition(tm, "q0", '1', "q0", '0', 'L');
    tm_add_transition(tm, "q1", '_', "q_acc", '_', 'S');

    strcpy(tm->start_state,  "q0");
    strcpy(tm->accept_state, "q_acc");
    strcpy(tm->reject_state, "q_rej");

    const Transition* tr = tm_find_transition(tm, "q0", '0');
    if (tr) {
        printf("\nNađen prijelaz: %s %c → %s %c %c\n",
               tr->current_state, tr->read_symbol,
               tr->next_state, tr->write_symbol, tr->move_dir);
    }

    tm_destroy(tm);

    return 0;
}