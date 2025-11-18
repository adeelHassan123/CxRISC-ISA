#ifndef PROGRAM_LOADER_H
#define PROGRAM_LOADER_H

#include "isa.h"

int load_program(const char *path, uint32_t imem[], int *count);

#endif