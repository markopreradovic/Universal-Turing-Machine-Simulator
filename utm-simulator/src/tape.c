#include "tape.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static TapeCell* create_cell(Symbol s) {
    TapeCell* cell = malloc(sizeof(TapeCell));
    if (!cell) {
        fprintf(stderr,"Out of memory!\n");
        exit(1);
    }
    cell->symbol = s;
    cell->left = NULL;
    cell->right = NULL;
    return cell;
}


Tape* tape_create(const char* initial_content, Symbol blank) {
    Tape* tape = malloc(sizeof(Tape));
    if (!tape) {
        fprintf(stderr,"Out of memory!\n");
        exit(1);
    }
    tape->blank = blank;
    tape->head = NULL;

    if (!initial_content || initial_content[0]=='\0') {
        tape->head = create_cell(blank); //Empty tape - only 1 blank cell
        return tape;
    }

    TapeCell* first = create_cell(initial_content[0]);
    tape->head = first;

    TapeCell* prev = first;
    for (size_t i = 1; initial_content[i]!='\0'; i++) {
        TapeCell* cell = create_cell(initial_content[i]);
        prev->right = cell;
        cell->left = prev;
        prev = cell;
    }

    tape->head = first; //Head on the start of the input, farleft cell
    return tape;
}

void tape_destroy(Tape* tape) {
    if (!tape) return;

    //Farleft cell
    TapeCell* current = tape->head;
    while (current && current->left) {
        current = current->left;
    }
    //Erasing everything from left to right
    while (current) {
        TapeCell* next = current->right;
        free(current);
        current = next;
    }
    free(tape);
}

Symbol tape_read(const Tape* tape) {
    if (!tape || !tape->head)
        return tape ? tape->blank : '_';
    return tape->head->symbol;
}

void tape_write(Tape *tape, Symbol s) {
    if (!tape || !tape->head) return;
    tape->head->symbol = s;
}

void tape_move_left(Tape* tape) {
    if (!tape || !tape->head) return;

    if (tape->head->left) {
        tape->head = tape->head->left;
    } else {
        // New blank cell left
        TapeCell* new_cell = create_cell(tape->blank);
        new_cell->right = tape->head;
        tape->head->left = new_cell;
        tape->head = new_cell;
    }
}

void tape_move_right(Tape* tape) {
    if (!tape || !tape->head) return;

    if (tape->head->right) {
        tape->head = tape->head->right;
    } else {
        // New blank cell right
        TapeCell* new_cell = create_cell(tape->blank);
        new_cell->left = tape->head;
        tape->head->right = new_cell;
        tape->head = new_cell;
    }
}

void tape_print(const Tape* tape, int visible_cells) {
    if (!tape || !tape->head) {
        printf("[empty tape]\n");
        return;
    }

    TapeCell* start = tape->head;
    for (int i = 0; i < visible_cells && start->left; i++) {
        start = start->left;
    }

    printf("Tape: ");

    TapeCell* current = start;
    int count = 0;

    while (current && count < visible_cells * 2 + 1) {
        char sym = current->symbol;
        if (sym == tape->blank) sym = '_';

        if (current == tape->head) {
            // Highlight head in red + bold
            printf("\033[1;31m[%c]\033[0m", sym);
        } else if (sym == '0') {
            printf("\033[34m%c\033[0m", sym);     // blue for '0'
        } else if (sym == '1') {
            printf("\033[32m%c\033[0m", sym);     // green for '1'
        } else {
            printf("%c", sym);                    // default for '_'
        }

        current = current->right;
        count++;
    }

    if (current) printf(" ...");
    printf("\n");
}

void tape_move(Tape* tape, char direction) {
    if (!tape || !tape->head) return;

    switch (direction) {
        case 'L':
            if (tape->head->left) {
                tape->head = tape->head->left;
            } else {
                TapeCell* new_cell = create_cell(tape->blank);
                new_cell -> right = tape->head;
                tape->head->left = new_cell;
                tape->head = new_cell;
                tape->head->left = new_cell;
            }
            break;

        case 'R':
            if (tape->head->right) {
                tape->head = tape->head->right;
            } else {
                TapeCell* new_cell = create_cell(tape->blank);
                new_cell->left = tape->head;
                tape->head->right = new_cell;
                tape->head = new_cell;
            }
            break;

        //No move, stay in place
        case 'S':
        case 'N':
            break;

        default:
            fprintf(stderr,"Unknown direction\n");
            break;
    }
}