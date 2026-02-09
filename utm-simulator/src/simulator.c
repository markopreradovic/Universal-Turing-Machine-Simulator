#include "simulator.h"
#include <stdio.h>
#include <string.h>

SimResult run_turing_machine(TuringMachine *tm, Tape *tape, long max_steps) {
    char current_state[MAX_STATE_NAME];
    strncpy(current_state, tm->start_state, MAX_STATE_NAME - 1);
    current_state[MAX_STATE_NAME - 1] = '\0';

    long step = 0;

    while (step<max_steps) {
        if (tm_is_accepting(tm,current_state)) return ACCEPTED;
        if (tm_is_rejecting(tm,current_state)) return REJECTED;

        Symbol symbol = tape_read(tape);
        const Transition *trans = tm_find_transition(tm, current_state, symbol);

        if (!trans) {
            return NO_TRANSITION;
        }

        tape_write(tape, trans->write_symbol);
        tape_move(tape, trans->move_dir);
        strncpy(current_state, trans->next_state, MAX_STATE_NAME - 1);
        current_state[MAX_STATE_NAME - 1] = '\0';
        step++;
    }
    return MAX_STEPS_EXCEEDED;
}