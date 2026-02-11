#include "simulator.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper functions

static uint64_t hash_tape_window(const Tape* tape, int radius) {
    uint64_t hash = 14695981039346656037ULL;   // FNV offset basis
    const uint64_t prime = 1099511628211ULL;   // FNV prime

    TapeCell* current = tape->head;

    // Move left 'radius' steps or as far as possible
    for (int i = 0; i < radius; i++) {
        if (!current->left) break;
        current = current->left;
    }

    // Hash the window from leftmost to rightmost
    int cells_hashed = 0;
    while (current && cells_hashed < radius * 2 + 1) {
        unsigned char c = (unsigned char)current->symbol;
        hash ^= c;
        hash *= prime;

        current = current->right;
        cells_hashed++;
    }

    return hash;
}

static bool config_set_init(ConfigSet* set, size_t initial_capacity) {
    set->configs = malloc(initial_capacity * sizeof(ExecutionConfig));
    if (!set->configs) return false;
    set->count = 0;
    set->capacity = initial_capacity;
    return true;
}

static void config_set_destroy(ConfigSet* set) {
    free(set->configs);
    set->configs = NULL;
    set->count = 0;
    set->capacity = 0;
}

static bool config_set_contains(const ConfigSet* set, const ExecutionConfig* config) {
    for (size_t i = 0; i < set->count; i++) {
        const ExecutionConfig* c = &set->configs[i];
        if (strcmp(c->state, config->state) == 0 &&
            c->head_position == config->head_position &&
            c->tape_hash == config->tape_hash) {
            return true;
        }
    }
    return false;
}

static bool config_set_add(ConfigSet* set, const ExecutionConfig* config) {
    if (set->count >= set->capacity) {
        // Double the capacity
        size_t new_cap = set->capacity ? set->capacity * 2 : 16;
        ExecutionConfig* new_mem = realloc(set->configs, new_cap * sizeof(ExecutionConfig));
        if (!new_mem) return false;
        set->configs = new_mem;
        set->capacity = new_cap;
    }

    set->configs[set->count] = *config;
    set->count++;
    return true;
}

static void print_step(
    long step,
    const char* current_state,
    long head_pos,
    Symbol read_symbol,
    const Transition* trans,
    const Tape* tape
) {
    if (!trans) {
        printf("Step %4ld | \033[33m%-6s\033[0m | pos %4ld | read \033[36m%c\033[0m   | \033[31mNO TRANSITION\033[0m\n",
               step, current_state, head_pos, read_symbol);
        return;
    }

    printf("Step %4ld | \033[33m%-6s\033[0m | pos %4ld | read \033[36m%c\033[0m   | "
           "write \033[35m%c\033[0m  \033[32m%c\033[0m   → \033[33m%-6s\033[0m | ",
           step, current_state, head_pos, read_symbol,
           trans->write_symbol, trans->move_dir, trans->next_state);

    tape_print(tape, 20);
}

// Main simulation function
SimResult run_turing_machine(
    const TuringMachine* tm,
    Tape* tape,
    long max_steps,
    bool step_mode           // true = pause after each step
) {
    char current_state[MAX_STATE_NAME];
    strncpy(current_state, tm->start_state, MAX_STATE_NAME - 1);
    current_state[MAX_STATE_NAME - 1] = '\0';

    long head_pos = 0;

    ConfigSet seen;
    if (!config_set_init(&seen, 1024)) {
        fprintf(stderr, "Failed to initialize config set\n");
        return MAX_STEPS_EXCEEDED;
    }

    long step = 0;

    printf("START | %-6s | pos %ld | Initial tape:\n", current_state, head_pos);
    tape_print(tape, 20);
    printf("\n");

    while (step < max_steps) {
        if (tm_is_accepting(tm, current_state)) {
            printf("→ ACCEPTED after %ld steps\n", step);
            config_set_destroy(&seen);
            return ACCEPTED;
        }
        if (tm_is_rejecting(tm, current_state)) {
            printf("→ REJECTED after %ld steps\n", step);
            config_set_destroy(&seen);
            return REJECTED;
        }

        Symbol symbol = tape_read(tape);

        ExecutionConfig current_config;
        strncpy(current_config.state, current_state, MAX_STATE_NAME - 1);
        current_config.state[MAX_STATE_NAME - 1] = '\0';
        current_config.head_position = head_pos;
        current_config.tape_hash = hash_tape_window(tape, TAPE_WINDOW_RADIUS);

        if (config_set_contains(&seen, &current_config)) {
            printf("→ INFINITE LOOP DETECTED after %ld steps "
                   "(repeated config: state=%s, pos=%ld)\n",
                   step, current_state, head_pos);
            config_set_destroy(&seen);
            return LOOP_DETECTED;
        }

        if (!config_set_add(&seen, &current_config)) {
            fprintf(stderr, "Failed to add configuration to set\n");
        }

        const Transition* trans = tm_find_transition(tm, current_state, symbol);

        print_step(step, current_state, head_pos, symbol, trans, tape);

        if (!trans) {
            printf("→ NO TRANSITION after %ld steps\n", step);
            config_set_destroy(&seen);
            return NO_TRANSITION;
        }

        tape_write(tape, trans->write_symbol);

        if (trans->move_dir == 'R') {
            head_pos++;
        } else if (trans->move_dir == 'L') {
            head_pos--;
        }

        tape_move(tape, trans->move_dir);

        strncpy(current_state, trans->next_state, MAX_STATE_NAME - 1);
        current_state[MAX_STATE_NAME - 1] = '\0';

        step++;

        if (step_mode) {
            printf("\n[Step %ld] Press Enter to continue (or Ctrl+C to stop)...\n", step);
            getchar();
        }
    }

    printf("→ MAX STEPS EXCEEDED (%ld steps)\n", max_steps);
    config_set_destroy(&seen);
    return MAX_STEPS_EXCEEDED;
}