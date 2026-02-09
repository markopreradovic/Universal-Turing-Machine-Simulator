#ifndef PARSER_H
#define PARSER_H

#include "tm.h"
#include <stdio.h>

bool load_tm_from_file(TuringMachine* tm, const char* filename);
#endif