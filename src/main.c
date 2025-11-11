#include <stdio.h>
#include "isa.h"
#include "registers.h"
#include "memory.h"
#include "program_loader.h"
#include "single_cycle.h"

#define PROGRAM_PATH "program/prog.asm"

int main() {
    Instr program[NUM_INST];  
    int prog_count = 0;

    // Step 1: Initialize registers and memory 
    regs_init();
    mem_init();

    // Step 2: Preload memory for testing
    mem_write(0, 5);
    mem_write(1, 3);

    // Step 3: Load program
    if (load_program(PROGRAM_PATH, program, &prog_count) != 0) {
        printf("Error: could not load program at '%s'\n", PROGRAM_PATH);
        return 1;
    }

    printf("Program loaded: %d instructions\n", prog_count);

    // Step 4: Run program
    run_program(program, prog_count);

    // Step 5: Dump final state
    printf("\n--- Final State ---\n");
    regs_dump();         
    mem_dump(0, 10);   

    return 0;
}