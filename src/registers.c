#include <stdio.h>
#include "isa.h"
#include "utils.h"

static int regs[NUM_REGS];

// Init all registers to zero
void regs_init() {
    for (int i = 0; i < NUM_REGS; i++)
        regs[i] = 0;
}

//Read data from registers
int regs_read(int index) {
    if (index < 0 || index >= NUM_REGS)
        return 0;
    return regs[index];
}

//Write data to registers
void regs_write(int index, int value) {
    if (index <= 0 || index >= NUM_REGS)
        return; 
    regs[index] = value;
}

// dump data of all registers
void regs_dump() {
    for (int i = 0; i < NUM_REGS; i++)
        printf("R%d = %d\n", i, regs[i]);
}



