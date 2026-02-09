#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "tm.h"
#include "tape.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#define TAPE_WINDOW_RADIUS 40

typedef enum {
    RUNNING,
    ACCEPTED,
    REJECTED,
    NO_TRANSITION,
    MAX_STEPS_EXCEEDED,
    LOOP_DETECTED
} SimResult;

typedef struct {
    char         state[MAX_STATE_NAME];
    long         head_position;     // currently relative position
    uint64_t     tape_hash;         // hash of local tape window
} ExecutionConfig;

typedef struct {
    ExecutionConfig* configs;
    size_t           count;
    size_t           capacity;
} ConfigSet;

SimResult run_turing_machine(
    const TuringMachine* tm,    // const - we don't modify the machine
    Tape*                tape,
    long                 max_steps
);

#endif // SIMULATOR_H