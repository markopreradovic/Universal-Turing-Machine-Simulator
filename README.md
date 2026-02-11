# Universal Turing Machine Simulator

## Project Description

This project implements a complete and fully functional **universal Turing machine (UTM) simulator** written in pure C. It is capable of simulating any single-tape deterministic Turing machine described in a simple custom text-based format (.tm files). The simulator faithfully emulates the classic Turing machine model: an infinite tape, a read/write head, a finite set of states, a transition table, and a blank symbol.

The simulator can load arbitrary Turing machines, execute them on given input strings, visualize every computation step, and detect common runtime behaviors such as acceptance, rejection, missing transitions, step limits exceeded, and infinite loops.

The project serves multiple purposes:
- Educational demonstration of the theoretical foundation of computation (Turing machines, Church-Turing thesis, decidability).
- Practical tool for experimenting with custom Turing machines and algorithms.
- Illustrative example of how to implement complex data structures (infinite tape via doubly-linked list) and algorithms (loop detection via configuration hashing) in low-level C with strong error handling and debuggability.
- Showcase of clean, modular C code following modern best practices (strict compiler warnings, CMake build system, clear separation of concerns).

The implementation is intentionally kept minimal yet powerful, using only the C standard library and no external dependencies beyond CMake for building.

## Purpose of the Project

This simulator is designed as a practical implementation of Alan Turing's foundational model of computation, providing a robust and extensible platform for exploring the principles of computability theory in an interactive and visual way.

### Primary Objectives
- To offer a complete, self-contained, and pedagogically valuable demonstration of how a universal Turing machine operates on arbitrary programs and inputs.
- To bridge theoretical concepts (infinite tape, state transitions, halting problem) with executable code, making abstract ideas tangible and observable.
- To serve as a reliable reference tool for students, educators, and researchers studying automata theory, formal languages, and the limits of algorithmic computation.

### Practical Applications
- **Education and Teaching**: Ideal for university courses on theory of computation, algorithms, and computability — students can load custom machines, observe execution traces, and experiment with halting and non-halting behavior.
- **Algorithm Prototyping**: Developers and theorists can prototype and debug small Turing machine programs (palindrome recognition, parity checking, increment, copying) before translating them into more complex models.
- **Demonstration of Undecidability**: The built-in loop detection mechanism provides a concrete approximation of the undecidable halting problem, illustrating why perfect loop detection is impossible in the general case.
- **Research and Experimentation**: Useful for testing small-scale Turing machines, exploring busy beaver-like behavior, or comparing different machine designs in terms of steps and tape usage.
- **Portfolio and Showcase**: Serves as a strong, well-documented example of clean C programming, modular design, strict error handling, visualization techniques, and command-line interface development.

The project emphasizes correctness, readability, debuggability, and visual clarity, making it suitable both for learning purposes and as a reference implementation of a universal Turing machine simulator.

## Project Structure

```
UTM-Simulator/
├── cmake-build-debug/              (CMake build directory – generated, ignored in git)
├── utm-simulator/
│   ├── examples/                   All ready-to-run .tm machine descriptions
│   │   ├── copy.tm                 Duplicates input string after a blank separator
│   │   ├── fast_loop.tm            Fast infinite loop for testing loop detection
│   │   ├── increment.tm            Binary increment (+1 to a binary number, LSB right)
│   │   ├── loop.tm                 Simple infinite right-moving loop on blanks
│   │   ├── palindrom.tm            Checks if input is a palindrome over {0,1}
│   │   ├── parity.tm               Accepts if number of '1's is even, rejects if odd
│   │   ├── real_loop.tm            Closed left-right infinite loop on blank
│   │   └── simple_loop.tm          Minimal loop for quick detection testing
│   └── src/                        All source code
│       ├── main.c                  Entry point, CLI argument parsing, machine loading, simulation launch
│       ├── tape.h / tape.c         Infinite bidirectional tape (doubly-linked list)
│       ├── tm.h / tm.c             Turing machine structure and transition table management
│       ├── parser.h / parser.c     Parser for .tm files (headers + transitions)
│       └── simulator.h / simulator.c Core simulation engine + loop detection + trace + step-by-step + breakpoints
└── CMakeLists.txt                  CMake build configuration (C11, strict warnings)
    .gitignore                      Git ignore rules
    README.md                       This file
```

### Detailed File Descriptions

#### src/main.c
- Program entry point.
- Parses command-line arguments (--file, --input, --max, --step, --break).
- Creates Turing machine instance.
- Loads .tm file using parser.
- Creates tape from input string.
- Runs simulation with selected parameters (step mode, breakpoints).
- Prints results, final tape state, and outcome (ACCEPTED, REJECTED, LOOP_DETECTED, etc.).
- Cleans up all allocated resources.

#### src/tape.h / tape.c
- Implements the infinite bidirectional tape as a doubly-linked list of cells.
- Each cell stores a symbol and pointers to left/right neighbors.
- Automatically allocates new blank cells when head moves beyond existing tape bounds.
- Provides functions for: create from string, destroy, read symbol, write symbol, move head (L/R/S), print visible window around head.
- Head position is tracked separately for loop detection.

#### src/tm.h / tm.c
- Defines the Turing machine data structure: start/accept/reject states, blank symbol, fixed-size array of transitions.
- Functions: create/destroy TM, add transition, find transition by current state and read symbol, check if state is accepting or rejecting.
- Uses fixed-size array for transitions (efficient for typical small machines).

#### src/parser.h / parser.c
- Reads and parses .tm files line by line.
- Handles header lines: blank: _, start: q0, accept: qacc, reject: qrej.
- Parses transition lines in format: state read -> next_state write direction.
- Skips empty lines and comments (#).
- Uses string trimming and sscanf for robust parsing.
- Calls tm_add_transition to populate the machine.

#### src/simulator.h / simulator.c
- Core execution engine.
- Main function run_turing_machine executes the simulation loop.
- Supports: early exit on accept/reject, breakpoint pauses, step-by-step mode (getchar pause), infinite loop detection.
- Loop detection: snapshots configuration (state + head position + FNV hash of ±40 cells), checks for repeats.
- Prints colored trace: state (yellow), read (cyan), write (magenta), move (green), head (red bold), 0 (blue), 1 (green), _ (gray).
- Returns one of: ACCEPTED, REJECTED, NO_TRANSITION, MAX_STEPS_EXCEEDED, LOOP_DETECTED.
- Handles resource cleanup in all exit paths.

## Features and How They Work

### Core Components

1. **Infinite Tape** (`tape.c`)
   - Implemented as doubly-linked list of cells.
   - Each cell contains a symbol and pointers to left/right neighbors.
   - Automatically expands with blank symbols when head moves beyond current bounds.
   - Supports read, write, move left/right/stay, create from string, destroy, and print window.

2. **Turing Machine Representation** (`tm.c`)
   - Fixed-size array of transitions (efficient for small machines).
   - Separate fields for start, accept, reject states and blank symbol.
   - Functions to add transition and find matching transition by state + read symbol.

3. **.tm File Parser** (`parser.c`)
   - Reads line by line, skips comments (#) and empty lines.
   - Parses header lines (`blank: _`, `start: q0`, `accept: qacc`, `reject: qrej`).
   - Parses transition lines in format: `state read -> next write dir`
   - Supports single-character symbols and state names up to 31 chars.

4. **Simulation Engine** (`simulator.c`)
   - Main loop: while step < max_steps
     - Early exit on accept/reject state.
     - Optional breakpoint check (pause on specified states).
     - Configuration snapshot: current state + head position + FNV-1a hash of ±40 cells around head.
     - Loop detection: if configuration repeats → return LOOP_DETECTED.
     - Read symbol, find transition, print colored step.
     - Execute write + move + update head position + change state.
     - Optional step-by-step pause (getchar() after each step).
   - Returns one of: ACCEPTED, REJECTED, NO_TRANSITION, MAX_STEPS_EXCEEDED, LOOP_DETECTED.

5. **Visualization**
   - Colored trace: states yellow, read cyan, write magenta, move green, next state yellow.
   - Tape window: head red & bold, 0 bright blue, 1 bright green, _ dim gray.
   - Clear separation of step info and tape content.

6. **Command Line Interface**
   - Supports:
     - `--file` / `-f` : path to .tm file
     - `--input` / `-i` : input string
     - `--max` / `-m` : maximum steps
     - `--step` / `-s` : enable step-by-step mode
     - `--break` / `-b` : comma-separated list of states to break on
   - Prints all parameters at startup.

7. **Error Handling & Robustness**
   - Strict compiler flags: `-Wall -Wextra -Wpedantic -Werror -Wshadow -Wconversion`
   - Checks for allocation failures, file loading errors, invalid transitions.
   - Clean resource cleanup in all exit paths.

### Included Example Turing Machines

| File                  | Description                                                                 | Sample Input   | Expected Result & Final Tape                     | Steps (approx.) |
|-----------------------|-----------------------------------------------------------------------------|----------------|--------------------------------------------------|-----------------|
| copy.tm               | Duplicates input string after blank separator                               | "1011"         | ACCEPTED, "1011_1011 [_]"                        | 30–50           |
| fast_loop.tm          | Fast infinite loop (used for detection testing)                             | ""             | INFINITE LOOP DETECTED                           | 2–5             |
| increment.tm          | Binary increment (+1), LSB on right                                         | "1011"         | ACCEPTED, "1100 [_]"                             | 8               |
| loop.tm               | Simple right-moving infinite loop on blanks                                 | "0"            | MAX STEPS EXCEEDED                               | 10000           |
| palindrome.tm         | Checks if input is palindrome over {0,1}                                    | "0110"         | ACCEPTED, "_XY1X[_]"                             | 16              |
| parity.tm             | Accepts if number of 1s is even, rejects if odd                             | "1001"         | ACCEPTED, "1001[_]"                              | 5               |
| real_loop.tm          | Closed left-right infinite loop on blank                                    | ""             | INFINITE LOOP DETECTED                           | 3–5             |
| simple_loop.tm        | Minimal loop for quick detection testing                                    | ""             | INFINITE LOOP DETECTED                           | 2               |

All examples are tested with step-by-step mode and colored output enabled.



Contributions, bug reports, and new .tm examples are welcome on GitHub.

Created as a project in C with focus on correctness, readability, and visualization of theoretical computation.
