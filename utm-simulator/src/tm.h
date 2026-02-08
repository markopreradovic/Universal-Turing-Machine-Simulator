#ifndef TM_H
#define TM_H

#include "tape.h"
#include <stdbool.h>

#define MAX_STATES      64
#define MAX_TRANSITIONS 256
#define MAX_STATE_NAME  32

typedef char StateName[MAX_STATE_NAME];

//One transition : (state,symbol) - > (new state, new symbol, direction)
typedef struct {
    StateName current_state;
    Symbol read_symbol;
    StateName next_state;
    Symbol write_symbol;
    char move_dir;   // L R S
} Transition;

typedef struct {
    Transition transitions[MAX_TRANSITIONS];
    int transition_count;
    StateName states[MAX_STATES];
    int state_count;
    StateName start_state;
    StateName accept_state;
    StateName reject_state;      // or halt-reject
    Symbol blank_symbol;
} TuringMachine;

//Initialization and free machine function
TuringMachine* tm_create(void);
void tm_destroy(TuringMachine* tm);

//Adding the transition
bool tm_add_transition(TuringMachine* tm, const char* curr_state, Symbol read_sym, const char* next_state, Symbol write_sym, char move_dir);

//Finding the transition for given state and symbol
const Transition* tm_find_transition(const TuringMachine* tm, const char* current_state, Symbol read_symbol);

bool tm_is_accepting(const TuringMachine* tm, const char* state);
bool tm_is_rejecting(const TuringMachine* tm, const char* state);

#endif