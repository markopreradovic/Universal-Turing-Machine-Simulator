#include "tape.h"
#include "tm.h"
#include "parser.h"
#include "simulator.h"
#include <stdio.h>

int main(void) {
    TuringMachine* tm = tm_create();
    if (!tm) {
        fprintf(stderr, "Failed to create Turing Machine\n");
        return 1;
    }

    const char* tm_file = "../utm-simulator/examples/increment.tm";
    const char* input = "1011";
    if (!load_tm_from_file(tm, tm_file)) {
        fprintf(stderr, "Failed to load TM file: %s\n", tm_file);
        tm_destroy(tm);
        return 1;
    }

    printf("Loaded machine:\n");
    printf("  start:       %s\n", tm->start_state);
    printf("  accept:      %s\n", tm->accept_state);
    printf("  reject:      %s\n", tm->reject_state);
    printf("  blank:       %c\n", tm->blank_symbol);
    printf("  transitions: %d\n\n", tm->transition_count);




    Tape* tape = tape_create(input, tm->blank_symbol);
    if (!tape) {
        fprintf(stderr, "Failed to create tape\n");
        tm_destroy(tm);
        return 1;
    }

    printf("Initial input: \"%s\"\n", input);
    printf("Initial tape:\n");
    tape_print(tape, 20);
    printf("\n");
    bool step_mode = true;
    SimResult result = run_turing_machine(tm, tape, 10000, step_mode);

    printf("\nFinal tape:\n");
    tape_print(tape, 20);

    printf("Result: ");
    switch (result) {
        case ACCEPTED:
            puts("ACCEPTED");
            break;
        case REJECTED:
            puts("REJECTED");
            break;
        case NO_TRANSITION:
            puts("NO TRANSITION");
            break;
        case MAX_STEPS_EXCEEDED:
            puts("MAX STEPS EXCEEDED");
            break;
        case LOOP_DETECTED:
            puts("INFINITE LOOP DETECTED");
            break;
        default:
            puts("UNKNOWN RESULT");
    }

    tape_destroy(tape);
    tm_destroy(tm);
    return 0;
}