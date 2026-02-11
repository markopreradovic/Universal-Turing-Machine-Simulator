#define _POSIX_C_SOURCE 200809L
#include "tape.h"
#include "tm.h"
#include "parser.h"
#include "simulator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]) {
    // Default values
    const char* tm_file = "../utm-simulator/examples/parity.tm";
    const char* input   = "1001";
    long max_steps      = 10000;
    bool step_mode      = true;
    Breakpoints* breakpoints = NULL;

    // CLI parsing
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--file") == 0 || strcmp(argv[i], "-f") == 0) {
            if (i + 1 < argc) tm_file = argv[++i];
        } else if (strcmp(argv[i], "--input") == 0 || strcmp(argv[i], "-i") == 0) {
            if (i + 1 < argc) input = argv[++i];
        } else if (strcmp(argv[i], "--max") == 0 || strcmp(argv[i], "-m") == 0) {
            if (i + 1 < argc) {
                max_steps = atol(argv[++i]);
                if (max_steps <= 0) max_steps = 10000;
            }
        } else if (strcmp(argv[i], "--step") == 0 || strcmp(argv[i], "-s") == 0) {
            step_mode = true;
        } else if (strcmp(argv[i], "--break") == 0 || strcmp(argv[i], "-b") == 0) {
            if (i + 1 < argc) {
                char* states_str = strdup(argv[++i]);
                char* token = strtok(states_str, ",");
                char** state_list = malloc(16 * sizeof(char*)); // max 16
                size_t count = 0;

                while (token && count < 16) {
                    state_list[count++] = strdup(token);
                    token = strtok(NULL, ",");
                }

                breakpoints = create_breakpoints((const char**)state_list, count);

                for (size_t j = 0; j < count; j++) free(state_list[j]);
                free(state_list);
                free(states_str);
            }
        } else {
            printf("Nepoznata opcija: %s\n", argv[i]);
        }
    }

    printf("Simulation parameters:\n");
    printf("  TM file:     %s\n", tm_file);
    printf("  Input:       \"%s\"\n", input);
    printf("  Max steps:   %ld\n", max_steps);
    printf("  Step mode:   %s\n", step_mode ? "ON" : "OFF");
    if (breakpoints) {
        printf("  Breakpoints: ");
        for (size_t i = 0; i < breakpoints->count; i++) {
            printf("%s%s", breakpoints->states[i], i < breakpoints->count-1 ? "," : "");
        }
        printf("\n");
    }
    printf("\n");

    TuringMachine* tm = tm_create();
    if (!tm) {
        fprintf(stderr, "Failed to create Turing Machine\n");
        destroy_breakpoints(breakpoints);
        return 1;
    }

    if (!load_tm_from_file(tm, tm_file)) {
        fprintf(stderr, "Failed to load TM file: %s\n", tm_file);
        tm_destroy(tm);
        destroy_breakpoints(breakpoints);
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
        destroy_breakpoints(breakpoints);
        return 1;
    }

    printf("Initial tape:\n");
    tape_print(tape, 20);
    printf("\n");

    SimResult result = run_turing_machine(tm, tape, max_steps, step_mode, breakpoints);

    printf("\nFinal tape:\n");
    tape_print(tape, 20);

    printf("Result: ");
    switch (result) {
        case ACCEPTED:          puts("ACCEPTED"); break;
        case REJECTED:          puts("REJECTED"); break;
        case NO_TRANSITION:     puts("NO TRANSITION"); break;
        case MAX_STEPS_EXCEEDED: puts("MAX STEPS EXCEEDED"); break;
        case LOOP_DETECTED:     puts("INFINITE LOOP DETECTED"); break;
        default:                puts("UNKNOWN RESULT");
    }

    destroy_breakpoints(breakpoints);
    tape_destroy(tape);
    tm_destroy(tm);
    return 0;
}