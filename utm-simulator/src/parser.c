#include "parser.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#define LINE_MAX 256

static char* trim(char* str) {
    char* end;
    while (isspace((unsigned char)*str)) str++;
    if (*str == 0) return str;

    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    *(end+1) = '\0';
    return str;
}

static bool parse_header(TuringMachine *tm, const char* line) {
    char key[64];
    char value[128];
    if (sscanf(line, "%63[^:]: %127[^\n]", key, value) != 2) {
        return false;
    }
    char* trimmed_key = trim(key);
    char* trimmed_value = trim(value);
    if (strcmp(trimmed_key, "blank") == 0) {
        tm->blank_symbol = trimmed_value[0];
        return true;
    }
    if (strcmp(trimmed_key, "start") == 0) {
        strncpy(tm->start_state, trimmed_value, MAX_STATE_NAME - 1);
        tm->start_state[MAX_STATE_NAME - 1] = '\0';
        return true;
    }
    if (strcmp(trimmed_key, "accept") == 0) {
        strncpy(tm->accept_state, trimmed_value, MAX_STATE_NAME - 1);
        tm->accept_state[MAX_STATE_NAME - 1] = '\0';
        return true;
    }
    if (strcmp(trimmed_key, "reject") == 0) {
        strncpy(tm->reject_state, trimmed_value, MAX_STATE_NAME - 1);
        tm->reject_state[MAX_STATE_NAME - 1] = '\0';
        return true;
    }

    return false;
}

static bool parse_transition(TuringMachine* tm, const char* line) {
    char curr[32], next[32];
    char read_sym, write_sym, dir;

    // Expected format:  q0 0 -> q1 1 R
    int count = sscanf(line, "%31s %c -> %31s %c %c", curr, &read_sym, next, &write_sym, &dir);

    if (count != 5) {
        return false;
    }
    return tm_add_transition(tm,
                             curr,
                             read_sym,
                             next,
                             write_sym,
                             dir);
}

bool load_tm_from_file(TuringMachine *tm, const char* filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Cant open file");
        return false;
    }

    char line[LINE_MAX];
    int line_num = 0;

    while (fgets(line, sizeof(line), fp)) {
        line_num++;
        char *trimmed = trim(line); //Skipping empty lines and comments
        if (*trimmed == '\0' || *trimmed == '#') {
            continue;
        }
        // Header line
        if (strchr(trimmed, ':')) {
            if (!parse_header(tm, trimmed)) {
                fprintf(stderr, "Error in line %d (header): %s\n", line_num, trimmed);
                fclose(fp);
                return false;
            }
            continue;
        }

        // Transitions
        if (strstr(trimmed, "->")) {
            if (!parse_transition(tm, trimmed)) {
                fprintf(stderr, "Error in line %d (transition): %s\n", line_num, trimmed);
                fclose(fp);
                return false;
            }
            continue;
        }

        fprintf(stderr, "Uknown format in line %d: %s\n", line_num, trimmed);
    }

    fclose(fp);
    return true;
}