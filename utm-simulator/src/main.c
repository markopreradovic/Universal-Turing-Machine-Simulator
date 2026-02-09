#include "tape.h"
#include "tm.h"
#include "parser.h"
#include <stdio.h>

int main(void) {
    TuringMachine* tm = tm_create();

    const char* tm_file = "../utm-simulator/examples/increment.tm";

    if (!load_tm_from_file(tm, tm_file)) {
        fprintf(stderr, "Neuspješno učitavanje TM fajla\n");
        tm_destroy(tm);
        return 1;
    }

    printf("Učitana mašina:\n");
    printf("  start:  %s\n", tm->start_state);
    printf("  accept: %s\n", tm->accept_state);
    printf("  reject: %s\n", tm->reject_state);
    printf("  blank:  %c\n", tm->blank_symbol);
    printf("  broj prijelaza: %d\n", tm->transition_count);

    // primjer traženja jednog prijelaza
    const Transition* tr = tm_find_transition(tm, tm->start_state, '1');
    if (tr) {
        printf("Primer prijelaza iz start stanja za '1': → %s %c %c\n",
               tr->next_state, tr->write_symbol, tr->move_dir);
    }

    tm_destroy(tm);
    return 0;
}