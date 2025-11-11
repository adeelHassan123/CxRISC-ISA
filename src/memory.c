#include <stdio.h>
#include "isa.h"
#include "utils.h"

static uint32_t mem[MEM_SIZE];

// Init all Memory blocks to zero
void mem_init() {
    for (int i = 0; i < MEM_SIZE; i++)
        mem[i] = 0;
}

// Read data from Memory
int mem_read(int addr) {
    if (addr < 0 || addr >= MEM_SIZE)
        return 0;
    return (int)mem[addr];
}

// Write data to memory
void mem_write(int addr, int value) {
    if (addr < 0 || addr >= MEM_SIZE)
        return;
    mem[addr] = (uint32_t)value;
}

// Print memory values
void mem_dump(int start, int end) {
    char bin[33];
    // Validate range
    if (start < 0 || end > MEM_SIZE || start >= end) {
        printf("Invalid range: %d to %d\n", start, end);
        return;
    }
    // Print memory in binary + decimal
    for (int i = start; i < end; i++) {
        int_to_binary(mem[i], bin);
        printf("[%d] = %s (%d)\n", i, bin, (int)mem[i]);
    }
}
