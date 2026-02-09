#include "tape.h"
#include "tm.h"
#include "parser.h"
#include "simulator.h"
#include <stdio.h>

int main(void) {
    TuringMachine* tm = tm_create();
    if (!load_tm_from_file(tm, "../utm-simulator/examples/increment.tm")) {
        tm_destroy(tm);
        return 1;
    }

    Tape* tape = tape_create("1011", tm->blank_symbol);

    printf("Početni ulaz: ");
    tape_print(tape, 15);

    SimResult result = run_turing_machine(tm, tape, 10000);

    printf("\nKraj izvršavanja:\n");
    tape_print(tape, 15);

    switch (result) {
        case ACCEPTED:          printf("→ ACCEPTED\n"); break;
        case REJECTED:          printf("→ REJECTED\n"); break;
        case NO_TRANSITION:     printf("→ Nema prijelaza\n"); break;
        case MAX_STEPS_EXCEEDED: printf("→ Prekoračen limit koraka\n"); break;
        default:                printf("→ Nepoznat rezultat\n");
    }

    tape_destroy(tape);
    tm_destroy(tm);
    return 0;
}