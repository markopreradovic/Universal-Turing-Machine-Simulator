#ifndef SIMULATOR_H
#define SIMULATOR_H
#include "tm.h"
#include "tape.h"

typedef enum {
    RUNNING,
    ACCEPTED,
    REJECTED,
    NO_TRANSITION,
    MAX_STEPS_EXCEEDED,
} SimResult;

SimResult run_turing_machine(TuringMachine *tm, Tape *tape, long max_steps);
#endif
