#include "tm.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

TuringMachine* tm_create(void) {
    TuringMachine *tm = calloc(1,sizeof(TuringMachine));
    if (!tm) {
        fprintf(stderr, "Cannot allocate TM");
        exit(1);
    }
    tm->blank_symbol = '_';
    return tm;
}

void tm_destroy(TuringMachine* tm) {
    free(tm);
}

bool tm_add_transition(TuringMachine* tm,
                       const char* curr_state,
                       Symbol read_sym,
                       const char* next_state,
                       Symbol write_sym,
                       char move_dir) {
    if (tm->transition_count >= MAX_TRANSITIONS) {
        fprintf(stderr, "Transition count exceeded");
        return false;
    }

    Transition* t = &tm->transitions[tm->transition_count++];
    strncpy(t->current_state, curr_state, MAX_STATE_NAME - 1);
    t->current_state[MAX_STATE_NAME - 1] = '\0';

    t->read_symbol = read_sym;

    strncpy(t->next_state, next_state, MAX_STATE_NAME - 1);
    t->next_state[MAX_STATE_NAME - 1] = '\0';
    t->write_symbol = write_sym;
    t->move_dir = move_dir;

    return true;
}

const Transition* tm_find_transition(const TuringMachine* tm, const char* current_state, Symbol read_symbol) {
    for (int i = 0; i < tm->transition_count; i++) {
        const Transition* t = &tm->transitions[i];
        if (strcmp(t->current_state, current_state) == 0 &&
            t->read_symbol == read_symbol) {
            return t;
            }
    }
    return NULL;
}

bool tm_is_accepting(const TuringMachine* tm, const char* state) {
    return strcmp(state, tm->accept_state) == 0;
}

bool tm_is_rejecting(const TuringMachine* tm, const char* state) {
    return strcmp(state, tm->reject_state) == 0;
}